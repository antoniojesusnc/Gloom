/**
@file Server.cpp

Contiene la implementaci�n del servidor de f�sica. 

@see Physics::CServer

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "Server.h"
#include "Conversions.h"
#include "ErrorManager.h"
#include "CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"
#include "Logic/Entity/Entity.h"

#include <assert.h>
#include <algorithm>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h>
#include <extensions\PxVisualDebuggerExt.h> 
#include <RepX\RepX.h>

using namespace Physics;
using namespace Logic;
using namespace physx;


// �nica instancia del servidor
CServer *CServer::_instance = NULL;

//--------------------------------------------------------

CServer::CServer() : _cudaContextManager(NULL), _scene(NULL)
{
	// Crear gestor de errores
	_errorManager = new CErrorManager();

	// Crear gestor de memoria
	_allocator = new PxDefaultAllocator();

	// Crear gestor de colisiones
	_collisionManager = new CCollisionManager();

	// Crear PxFoundation. Es necesario para instanciar el resto de objetos de PhysX
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);
	assert(_foundation && "Error en PxCreateFoundation");

	// Crear PxProfileZoneManager. Es necesario para habitiar algunas opciones de 
	// profiling de rendimiento en el PhysX Visual Debugger 
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	assert(_profileZoneManager && "Error en PxProfileZoneManager::createProfileZoneManage");

	// Crear PxPhysics. Es el punto de entrada al SDK de PhysX
	PxTolerancesScale toleranceScale;
	bool recordMemoryAllocations = true;
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, toleranceScale, 
		                       recordMemoryAllocations, _profileZoneManager);
	assert(_physics && "Error en PxCreatePhysics");

	// Crear CudaContextManager. Permite aprovechar la GPU para hacer parte de la simulaci�n f�sica.
	// Se utiliza posteriormente al crear la escena f�sica.
	// S�lo Windows
#ifdef PX_WINDOWS

	pxtask::CudaContextManagerDesc cudaContextManagerDesc;
	_cudaContextManager = pxtask::createCudaContextManager(*_foundation, cudaContextManagerDesc, 
		                                                   _profileZoneManager);
	if( _cudaContextManager )
	{
		if( !_cudaContextManager->contextIsValid() )
		{
			_cudaContextManager->release();
			_cudaContextManager = NULL;
		}
	}

#endif

	// Crear PxControllerManager. Es necesario para crear character controllers
	_controllerManager = PxCreateControllerManager(*_foundation);

	// Inicializar el m�dulo PxCooking. Es necesario para cocinar mallas y para 
	// deserializar actores a partir de ficheros RepX
	PxCookingParams params;
	_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);

	// Crear el material que se usar� por defecto
	float staticFriction = 0.5f;
	float dynamicFriction = 0.5f;
	float restitution = 0.1f;
	_defaultMaterial = _physics->createMaterial(staticFriction, dynamicFriction, restitution);

	// Intentar conectar con PhysX Visual Debugger (PVD)
	_pvdConnection = NULL;

	// S�lo en modo DEBUG
#ifdef _DEBUG
	debugger::comm::PvdConnectionManager *pvdConnectionManager = _physics->getPvdConnectionManager();
	assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");

	// Configurar m�quina, puerto y tiempo de espera (en millisegundos)
	const char *ip = "127.0.0.1";
	int port = 5425;						
	unsigned int timeout = 100;				

	// Configurar qu� informaci�n queremos mandar al PVD (debug, profile, memory)
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	// Intentar establecer la conexi�n
	_pvdConnection = PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);

#endif
} 

//--------------------------------------------------------

CServer::~CServer() 
{
	// Destruir objetos en orden inverso a como fueron creados
	if (_pvdConnection) {
		_pvdConnection->release();
		_pvdConnection = NULL;
	}

	if (_defaultMaterial) {
		_defaultMaterial->release();
		_defaultMaterial = NULL;
	}

	if (_cooking) {
		_cooking->release();
		_cooking = NULL;
	}

	if (_controllerManager) {
		_controllerManager->release();
		_controllerManager = NULL;
	}

	if (_cudaContextManager) {
		_cudaContextManager->release();
		_cudaContextManager = NULL;
	}

	if (_physics) {
		_physics->release();
		_physics = NULL;
	}

	if (_profileZoneManager) { 
		_profileZoneManager->release();
		_profileZoneManager = NULL;
	}

	if (_foundation) {
		_foundation->release();
		_foundation = NULL;
	}
	
	if (_collisionManager) {
		delete _collisionManager;
		_collisionManager = NULL;
	}

	if (_allocator) {
		delete _allocator;
		_allocator = NULL;
	}

	if (_errorManager) {
		delete _errorManager;
		_errorManager = NULL;
	}
} 

//--------------------------------------------------------

bool CServer::Init() 
{
	if (!_instance) {
		_instance = new CServer();
	}


	return true;
} 

//--------------------------------------------------------

void CServer::Release()
{
	if(_instance) {
		delete _instance;
		_instance = NULL;
	}
} 

//--------------------------------------------------------

void CServer::createScene ()
{
	assert(_instance);
	
	// Crear el descriptor de la escena
	PxSceneDesc sceneDesc(_physics->getTolerancesScale());

	// Establecer la gravedad en el eje Y
	sceneDesc.gravity = PxVec3(0.0f, -9.81f * 5, 0.0f);

	// Establecer el gestor de colisiones
	sceneDesc.simulationEventCallback = _collisionManager;

	// Establecer un gestor de tareas por CPU
	if (!sceneDesc.cpuDispatcher) {
		int mNbThreads = 1; // hilos de ejecuci�n
		_cpuDispatcher  = PxDefaultCpuDispatcherCreate(mNbThreads);
		assert (_cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

		sceneDesc.cpuDispatcher = _cpuDispatcher;
	}

	// Establecer el shader que controla las colisiones entre entidades.
	// Usamos un shader que emula la gesti�n de grupos de PhysX 2.8
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// Intentar establecer un gestor de tareas por GPU 
	// S�lo Windows
#ifdef PX_WINDOWS
	if (!sceneDesc.gpuDispatcher && _cudaContextManager)
	{
		sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
	}
#endif

	// Crear la escena f�sica
	_scene = _physics->createScene(sceneDesc);
	_acumTime=0;
	_fixedTime=5;
	assert(_scene && "Error en PxPhysics::createScene");

	//Establecemos los grupos de colision
	//Nada por el momento
}

//--------------------------------------------------------

void CServer::destroyScene ()
{
	assert(_instance);

	if (_scene) {
		_scene->release();
		_scene = NULL;
	}
}

//--------------------------------------------------------

bool CServer::tick(unsigned int msecs) 
{
	assert(_scene);
	_acumTime+=msecs;
	// Empezar la simulaci�n f�sica. Actualmente usamos intervalos de tiempo variables,
	// debemos tener cuidado porque PhysX puede no comportarse bien si el tiempo 
	// transcurrido es demasiado grande.
	// Se ha cambiado a intervalos fijos y si nos pasamos hacemos repeticiones del intervalo fijo para la CCD
	while(_acumTime>=_fixedTime){
		_scene->simulate(_fixedTime / 1000.0f);
		_acumTime-=5;
		if(_acumTime>=_fixedTime)
			_scene->fetchResults(true);
	}
	// Esperamos a que la simulaci�n f�sica termine. En principio podr�amos utilizar
	// este intervalo de tiempo para hacer algo m�s �til. Existe una versi�n de este
	// m�todo no bloqueante.
	return _scene->fetchResults(true);
} 

//--------------------------------------------------------

PxRigidStatic* CServer::createPlane(const Vector3 &point, const Vector3 &normal, int group, 
	                                const IPhysics *component)
{
	assert(_scene);

	// Crear un plano est�tico
	PxPlane plane(Vector3ToPxVec3(point), Vector3ToPxVec3(normal));
	PxMaterial *material = _defaultMaterial;
	PxRigidStatic *actor = PxCreatePlane(*_physics, plane, *material);
	
	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);
	
	// A�adir el actor a la escena
	_scene->addActor(*actor);

	return actor;
}

//--------------------------------------------------------

PxRigidStatic* CServer::createStaticBox(const Vector3 &position, const Vector3 &dimensions, bool trigger, 
	                                    int group, const IPhysics *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformaci�n local, por lo que la conversi�n entre sistemas de coordenadas es transparente. 
	
	// Crear un cubo est�tico
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
	PxMaterial *material = _defaultMaterial;
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX
	PxRigidStatic *actor = PxCreateStatic(*_physics, pose, geom, *material, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);

	// A�adir el actor a la escena
	_scene->addActor(*actor);
	
	return actor;
}

//--------------------------------------------------------

PxRigidDynamic* CServer::createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
	                                      float mass, bool kinematic, bool trigger, int group, 
										  const IPhysics *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformaci�n local, por lo que la conversi�n entre sistemas de coordenadas es transparente. 
	// Crear un cubo din�mico
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
	PxMaterial *material = _defaultMaterial;
	float density = mass / (dimensions.x * dimensions.y * dimensions.z);
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX

	// Crear cubo din�mico o cinem�tico
	PxRigidDynamic *actor;
	if (kinematic)
		actor = PxCreateKinematic(*_physics, pose, geom, *material, density, localPose);
	else
		actor = PxCreateDynamic(*_physics, pose, geom, *material, density, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);

	// A�adir el actor a la escena
	_scene->addActor(*actor);

	return actor;
}

//--------------------------------------------------------

physx::PxRigidStatic* CServer::createStaticSphere(const Vector3 &position, float radius, 
			                                      bool trigger, int group, const Logic::IPhysics *component) {

	assert(_scene);

	// Creamos una esfera est�tica
	PxTransform pose(Vector3ToPxVec3(position));
	PxSphereGeometry geom(radius);
	PxMaterial *material = _defaultMaterial;
	PxTransform localPose(PxVec3(0, radius, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX
	PxRigidStatic *actor = PxCreateStatic(*_physics, pose, geom, *material, localPose);

	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);

	// A�adir el actor a la escena
	_scene->addActor(*actor);
	
	return actor;
}

//--------------------------------------------------------

PxRigidDynamic* CServer::createDynamicSphere(const Vector3 &position, float radius, 
	                                         float mass, bool kinematic, bool trigger, int group, 
										     const IPhysics *component) {

	assert(_scene);

	// Creamos una esfera din�mica
	PxTransform pose(Vector3ToPxVec3(position));
	PxSphereGeometry geom(radius);
	PxMaterial *material = _defaultMaterial;
	float density = mass / (4.0/3.0 * 3.141592653589793 * radius * radius * radius);
	PxTransform localPose(PxVec3(0, radius, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX

	// Crear esfera din�mico o cinem�tico
	PxRigidDynamic *actor;
	if (kinematic)
		actor = PxCreateKinematic(*_physics, pose, geom, *material, density, localPose);
	else
		actor = PxCreateDynamic(*_physics, pose, geom, *material, density, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);

	// A�adir el actor a la escena
	_scene->addActor(*actor);

	return actor;
}

//--------------------------------------------------------

PxRigidActor* CServer::createFromFile(const std::string &file, int group, const IPhysics *component)
{
	assert(_scene);

	// Preparar par�metros para deserializar
	PxDefaultFileInputData data(file.c_str());
	PxCollection* bufferCollection = _physics->createCollection();
	PxCollection* sceneCollection = _physics->createCollection();
	PxStringTable* stringTable = NULL; 
	PxUserReferences* externalRefs = NULL; 
	PxUserReferences* userRefs = NULL; 

	// Deserializar a partir del fichero RepX
	repx::deserializeFromRepX(data, *_physics, *_cooking, stringTable, externalRefs, 
		                      *bufferCollection, *sceneCollection, userRefs);
	
	// A�adir entidades f�sicas a la escena
	_physics->addCollection(*sceneCollection, *_scene); 
	
	// Buscar una entidad de tipo PxRigidActor. Asumimos que hay exactamente 1 en el fichero.
	PxRigidActor *actor = NULL;
	for (unsigned int i=0; (i<sceneCollection->getNbObjects()) && !actor; i++) {
		PxSerializable *p = sceneCollection->getObject(i);
		actor = p->is<PxRigidActor>();
		
	}
	assert(actor);
	
	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	PxSetGroup(*actor, group);

	// Liberar recursos
	bufferCollection->release();
	sceneCollection->release();

	return actor;
}

//--------------------------------------------------------

void CServer::destroyActor(physx::PxActor *actor)
{
	assert(_scene);

	// Eliminar el actor de la escena
	_scene->removeActor(*actor);

	// Liberar recursos
	actor->release();
}

//--------------------------------------------------------

Matrix4 CServer::getActorTransform(const PxRigidActor *actor)
{
	assert(actor);

	// Devolver la posici�n y orientaci�n en coordenadas l�gicas
	return PxTransformToMatrix4(actor->getGlobalPose());
}

//--------------------------------------------------------

void CServer::moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform)
{
	assert(actor);
	assert(isKinematic(actor));

	// Mover el actor tras transformar el destino a coordenadas l�gicas
	actor->setKinematicTarget(Matrix4ToPxTransform(transform));
}

//--------------------------------------------------------

void CServer::moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ)
{
	assert(actor);
	assert(isKinematic(actor));

	// Desplazar el actor
	PxTransform transform = actor->getGlobalPose();
	transform.p += Vector3ToPxVec3(displ);
	actor->setKinematicTarget(transform);
}

//--------------------------------------------------------

bool CServer::isKinematic(const PxRigidDynamic *actor)
{
	assert(actor);

	return actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
}

//--------------------------------------------------------

PxCapsuleController* CServer::createCapsuleController(const Vector3 &position, float radius, 
	                                                  float height, const CPhysicController *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la c�psula, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en los pi�s del 
	// jugador. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Desafortunadamente, el descriptor que se usa para crear los controllers no permite
	// definir esta transformaci�n local (que s� permite al crear un actor), por lo que
	// tendremos que realizar la traslaci�n nosotros cada vez. 

	// Transformaci�n entre el sistema de coordenadas l�gico y el de PhysX
	float offsetY = height / 2.0f + radius;
	PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));
	
	// Crear descriptor del controller
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
	desc.height = height;
	desc.radius = radius;
	desc.material = _defaultMaterial;
	desc.climbingMode = PxCapsuleClimbingMode::eEASY;
	//desc.slopeLimit = 0.707f;
	desc.callback = _collisionManager;   // Establecer gestor de colisiones
	desc.userData = (void *) component;  // Anotar el componente l�gico asociado al controller
	PxCapsuleController *controller = (PxCapsuleController *)
		 _controllerManager->createController(*_physics, _scene, desc);
	

	// Anotar el componente l�gico asociado al actor dentro del controller (No es autom�tico)
	controller->getActor()->userData = (void *) component;

	//Establecemos el grupo del jugador al 3
	PxSetGroup(*controller->getActor(), 3);

	return controller;
}

//--------------------------------------------------------

unsigned CServer::moveController(PxController *controller, const Vector3 &movement, unsigned int msecs)
{
	assert(_scene);

	// Mover el character controller y devolver los flags de colisi�n
	PxVec3 disp = Vector3ToPxVec3(movement);
	float minDist = 0.01f;
	float elapsedTime = msecs / 1000.0f;
	PxControllerFilters filters;
	PxObstacleContext *obstacles = NULL;
	return controller->move(disp, minDist, elapsedTime, filters, obstacles);
}

//--------------------------------------------------------

Vector3 CServer::getControllerPosition(const PxCapsuleController *controller)
{
	assert(_scene);

	// Antes de devolver la posici�n del controller debemos transformar entre el 
	// sistema de coordenadas de PhysX y el de la l�gica
	float offsetY = controller->getHeight() / 2.0f + controller->getRadius();
	Vector3 pos = PxExtendedVec3ToVector3(controller->getPosition());
	return pos - Vector3(0, offsetY, 0);
}

//--------------------------------------------------------

void CServer::setControllerPosition(PxCapsuleController *controller, const Vector3 &position)
{
	assert(_scene);
	// Transformaci�n entre el sistema de coordenadas l�gico y el de PhysX
	float offsetY = controller->getHeight() / 2.0f + controller->getRadius();
	PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));
	PxExtendedVec3 posicionPhysics(pos.x,pos.y,pos.z);
	controller->setPosition(posicionPhysics);

}

//--------------------------------------------------------

void CServer::setRigidBodyPosition(physx::PxRigidBody* actor, const Vector3& position) {
	assert(_scene);
	
	// Transformaci�n entre el sistema de coordenadas l�gico y el de PhysX

	// En primer lugar obtenemos todas las formas del actor y calculamos el punto medio
	// de todas ellas.
	unsigned int nbShapes = actor->getNbShapes(); // sacamos el numero de formas del actor
	PxShape** actorShapes = new PxShape* [nbShapes]; // creamos un array de shapes
	actor->getShapes(actorShapes, nbShapes); // obtenemos todas las formas del actor
	float averageYPosition = 0; // Contendra la altura media de todos los shapes

	for(int i = 0; i < nbShapes; ++i) {
		PxGeometryType::Enum geomType = actorShapes[i]->getGeometryType();

		if(geomType == PxGeometryType::eSPHERE) {
			PxSphereGeometry sphereGeom; 
			actorShapes[i]->getSphereGeometry(sphereGeom);

			averageYPosition += sphereGeom.radius;
		}
		else if(geomType == PxGeometryType::eCAPSULE) {
			PxCapsuleGeometry capsuleGeom; 
			actorShapes[i]->getCapsuleGeometry(capsuleGeom);

			averageYPosition += capsuleGeom.halfHeight;
		}
		else if(geomType == PxGeometryType::eBOX) {
			PxBoxGeometry boxGeom; 
			actorShapes[i]->getBoxGeometry(boxGeom);

			PxVec3 halfPos(boxGeom.halfExtents);
			averageYPosition += halfPos.y;
		}
		/*else if(geomType == PxGeometryType::eTRIANGLEMESH) {
			// Deducir punto medio del mesh
		}*/
	}

	// Calculamos la altura media de todas las formas para colocar el vector
	// posicion de physx
	averageYPosition = averageYPosition / nbShapes;
	actor->setGlobalPose( PxTransform( PxVec3(position.x, position.y + averageYPosition, position.z) ) );
	//actor->setGlobalPose( PxTransform( PxVec3(position.x, position.y, position.z) ) );
}

//--------------------------------------------------------

void CServer::setGroupCollisions(int group1, int group2, bool enable)
{
	// Activar / desactivar colisiones entre grupos
	PxSetGroupCollisionFlag(group1, group2, enable);
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosest (const Ray& ray, float maxDist) const
{
	assert(_scene);

	// Establecer par�mettros del rayo
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // direcci�n normalizada   
	PxReal maxDistance = maxDist;                          // distancia m�xima
	PxRaycastHit hit;                 

	// Informaci�n que queremos recuperar de la posible colisi�n
	//const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
	const PxSceneQueryFlags outputFlags;

	// Lanzar el rayo
	bool intersection = _scene->raycastSingle(origin, unitDir, maxDistance, outputFlags, hit);
	
	

	// IMPORTANTE: aunque se haya llamado al m�todo move de los controllers y al consultar su posici�n
	// esta aparezca actualizada, sus actores asociados no se habr�n desplazado a�n. La consecuencia
	// es que se pueden recuperar colisiones inesperadas.

	if (intersection) {
		// Devolver entidad l�gica asociada a la entidad f�sica impactada
		IPhysics *component = (IPhysics *) hit.shape->getActor().userData;
		return component->getEntity();
	} else {
		return NULL;
	}
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosest(const Ray& ray, float maxDist, int group) const 
{
	assert(_scene);

	// Establecer par�mettros del rayo
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // direcci�n normalizada   
	PxReal maxDistance = maxDist;                          // distancia m�xima
	PxRaycastHit hit;                 
	const PxSceneQueryFlags outputFlags;				   // Info que queremos recuperar	

	// Lanzar el rayo
	PxRaycastHit hits[64];
	bool blockingHit;
	PxI32 nHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 64, blockingHit); 

	// Buscar un actot que pertenezca al grupo de colisi�n indicado
	for (int i=0; i<nHits; i++) {
		PxRigidActor *actor = &hits[i].shape->getActor();
		if (PxGetGroup(*actor) == group) {
			IPhysics *component = (IPhysics *) actor->userData;
			if (component) {
				return component->getEntity();
			}
		}
	}

	return NULL;

	// Nota: seguro que se puede hacer de manera mucho m�s eficiente usando los filtros
	// de PhysX.
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosestInverse(const Ray& ray, float maxDist, int group) const 
{
	assert(_scene);

	// Establecer par�mettros del rayo
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // direcci�n normalizada   
	PxReal maxDistance = maxDist;                          // distancia m�xima
	PxRaycastHit hit;                 
	const PxSceneQueryFlags outputFlags;				   // Info que queremos recuperar	

	// Lanzar el rayo
	PxRaycastHit hits[60];
	bool blockingHit;

	PxI32 nHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 60, blockingHit); 
	
	// Buscar un actot que pertenezca al grupo de colisi�n indicado
	for (int i=nHits-1; i>=0; i--) {
		PxRigidActor *actor = &hits[i].shape->getActor();
		if (PxGetGroup(*actor) != group) {
			IPhysics *component = (IPhysics *) actor->userData;
			if (component) {
				return component->getEntity();
			}
		}
	}

	return NULL;

	// Nota: seguro que se puede hacer de manera mucho m�s eficiente usando los filtros
	// de PhysX.
}

//--------------------------------------------------------

void CServer::addImpulsiveForce( PxRigidDynamic* actor, const Vector3& force ) {
	actor->addForce( Vector3ToPxVec3(force), PxForceMode::eIMPULSE );
}
