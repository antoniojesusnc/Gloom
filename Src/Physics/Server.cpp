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

	// TODO: crear gestor de colisiones
	_collisionManager = new CCollisionManager();

	// TODO: Crear PxFoundation
	// Usar nuestro gestor de memoria y nuestro gestor de errores
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);

	// TODO: Crear PxProfileZoneManager
	// Es necesario para habitiar algunas opciones de profiling de rendimiento en el PhysX Visual Debugger 
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);

	// TODO: Crear PxPhysics
	// Es el punto de entrada al SDK de PhysX
	PxTolerancesScale toleranceScale;
	bool recordMemoryAllocations = true;
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation,
							   PxTolerancesScale(), recordMemoryAllocations, _profileZoneManager );


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

	// TODO: crear controller manager (PxCreateControllerManager)
	_controllerManager = PxCreateControllerManager(*_foundation);

	// TODO: inicializar el m�dulo PxCooking
	// Es necesario para cocinar mallas y para deserializar actores a partir de ficheros RepX
	// Usar la funci�n PxCreateCooking
	PxCookingParams params;
	_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);

	// TODO: crear un material por defecto
	// Usar el m�todo adecuado del objeto PxPhysics
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

	// TODO: Liberar material por defecto (release)
	if(_defaultMaterial) {
		_defaultMaterial->release();
		_defaultMaterial = NULL;
	}

	// TODO: Liberar el m�dulo de cooking (release)
	if(_cooking) {
		_cooking->release();
		_cooking = NULL;
	}
	
	// TODO: Liberar el controller manager (release)
	if(_controllerManager) {
		_controllerManager->release();
		_controllerManager = NULL;
	}

	if (_cudaContextManager) {
		_cudaContextManager->release();
		_cudaContextManager = NULL;
	}

	// TODO: Liberar objeto PxPhysics (release)
	if(_physics) {
		_physics->release();
		_physics = NULL;
	}

	// TODO: Liberar objeto PxProfileZoneManager (release)
	if(_profileZoneManager) {
		_profileZoneManager->release();
		_profileZoneManager = NULL;
	}

	// TODO: Liberar objeto PxFoundation (release)
	if(_foundation) {
		_foundation->release();
		_foundation = NULL;
	}

	// TODO: destruir gestor de colisiones
	if(_collisionManager) {
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
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// TODO: establecer el gestor de colisiones
	// Asignar el gestor de colisiones al atributo simulationEventCallback
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

	// TODO: Crear la escena f�sica
	// Usando el m�todo adecuado del objeto PxPhysics
	_scene = _physics->createScene(sceneDesc);
	//if(!_scene)
	//	fatalError("createScene failed");
}

//--------------------------------------------------------

void CServer::destroyScene ()
{
	assert(_instance);

	// TODO: Liberar la escena (m�todo release)
	if(_scene) {
		_scene->release();
		_scene = NULL;
	}
}

//--------------------------------------------------------

bool CServer::tick(unsigned int msecs) 
{
	assert(_scene);

	// Empezar la simulaci�n f�sica. Actualmente usamos intervalos de tiempo variables,
	// debemos tener cuidado porque PhysX puede no comportarse bien si el tiempo 
	// transcurrido es demasiado grande.
	_scene->simulate(msecs / 1000.0f);

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

	// TODO: Crear un plano est�tico
	// 1. Transformar el punto y la normal a los tipos de PhysX
	PxVec3 pxPoint = Vector3ToPxVec3(point);
	PxVec3 pxNormal = Vector3ToPxVec3(normal);
	// 2. Usar el material por defecto del servidor
	// 3. Crear el actor usando PxCreatePlane
	PxPlane plane(pxPoint, pxNormal);
	PxRigidStatic *actor = PxCreatePlane(*_physics, plane, *_defaultMaterial);
	
	// TODO: enlazar el actor f�sico al componente l�gico
	// Usamos el atributo userData del actor para guardar la direcci�n del componente
	// l�gico encargado de la f�sica. 
	actor->userData = (void*) component;

	// TODO: Establecer el grupo de colisi�n
	// Usar la funci�n PxSetGroup
	PxSetGroup(*actor, group);
	
	// TODO: A�adir el actor a la escena
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
	
	// TODO: Crear un cubo est�tico
	// 0. Recuerda que siempre debes convertir entre los tipos de la l�gica y los de PhysX (vectores, etc).
	// 1. Crear pose (PxTransform) a partir de la posici�n
	PxTransform pxPosition( Vector3ToPxVec3(position) );
	// 2. Crear geometr�a de la caja (PxBoxGeometry) a partir de las dimensiones
	PxBoxGeometry pxGeom( Vector3ToPxVec3(dimensions) );
	// 3. Usar el material por defecto del servidor de f�sica
	// 4. Aplicar una transformaci�n local (localPose) que desplace dimensions.y hacia arriba (eje Y positivo)
	// 5. Crear el actor usando la funci�n PxCreateStatic
	PxTransform pxLocalPosition ( PxVec3(0, dimensions.y, 0) );
	PxRigidStatic *actor = PxCreateStatic(*_physics, pxPosition, pxGeom, *_defaultMaterial, pxLocalPosition);
	
	// TODO: Transformar el objeto est�tico en un trigger si es necesario
	// 1. Obtener la primera shape del actor
	// 2. Activar el flag PxShapeFlag::eTRIGGER_SHAPE
	if(trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// TODO: enlazar el actor f�sico al componente l�gico
	// Usamos el atributo userData del actor para guardar la direcci�n del componente
	// l�gico encargado de la f�sica.
	actor->userData = (void*) component;

	// TODO: Establecer el grupo de colisi�n
	// Usar la funci�n PxSetGroup
	PxSetGroup(*actor, group);

	// TODO: A�adir el actor a la escena
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

	// TODO: Crear cubo din�mico o cinem�tico (seg�n el par�metro kinematic)
	// - Para din�mico usar PxCreateDynamic
	// - Para cinematico usar PxCreateKinematic
	// Asignarlo al actor;
	PxRigidDynamic *actor = NULL;
	if(kinematic)
		actor = PxCreateKinematic(*_physics, pose, geom, *material, density, localPose);
	else
		actor = PxCreateDynamic(*_physics, pose, geom, *material, density, localPose);
	
	// TODO: Transformar el objeto din�mico en un trigger si es necesario
	// 1. Obtener la primera shape del actor
	// 2. Activar el flag PxShapeFlag::eTRIGGER_SHAPE

	// TODO: enlazar el actor f�sico al componente l�gico
	// Usamos el atributo userData del actor para guardar la direcci�n del componente
	// l�gico encargado de la f�sica. 
	actor->userData = (void*) component;

	// TODO: Establecer el grupo de colisi�n
	// Usar la funci�n PxSetGroup
	PxSetGroup(*actor, group);

	// TODO: a�adir el actor a la escena
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
	
	// TODO: enlazar el actor f�sico al componente l�gico
	// Usamos el atributo userData del actor para guardar la direcci�n del componente
	// l�gico encargado de la f�sica. 
	actor->userData = (void*) component;

	// TODO: Establecer el grupo de colisi�n 
	// Usar la funci�n PxSetGroup
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

	// TODO: desplazar el actor cinem�tico
	// 1. Obtener la matriz de transformaci�n del actor
	// 2. Aplicarle el desplazamiento para obtener la posici�n de destino 
	//    (conversi�n de coordenada l�gicas a f�sicas)
	// 3. Mover el actor f�sico a la posici�n de destino usando el m�todo 
	//    adecuado para actores cinem�ticos.
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

	// TODO: transformar entre distemas de coordenadas l�gico y de PhysX
	// 1. offsetY = altura / 2 + radio
	// 2. Transformar entre vector l�gico y vector de PhysX
	float offsetY = height / 2.0f + radius;
	PxExtendedVec3 p = Vector3ToPxExtendedVec3(position) + PxExtendedVec3(0, offsetY, 0);

	// TODO: crear descriptor de controller de tipo c�psula (PxCapsuleControllerDesc)
	// - usar material por defecto del servidor (_defaultMaterial)
	// - usar climbingMode PxCapsuleClimbingMode::eEASY
	PxCapsuleControllerDesc desc;
	desc.height = height;
	desc.radius = radius;
	desc.material = _defaultMaterial;
	desc.climbingMode = PxCapsuleClimbingMode::eEASY;
	desc.position = p;

	// TODO: asociar el gestor de colisiones al controller
	// Asignar el gestor de colisiones al atributo callback del descriptor del controller
	desc.callback = _collisionManager;

	// TODO: enlazar el controller f�sico al componente l�gico
	// Usamos el atributo userData del descriptor del controller para guardar la 
	// direcci�n del componente l�gico encargado de la f�sica.
	desc.userData = (void*) component;

	// TODO: crear controller a partir del _controllerManager
	PxCapsuleController *controller = (PxCapsuleController*) _controllerManager->createController(*_physics, _scene, desc);

	// TODO: enlazar el actor f�sico del controller al componente l�gico
	// Usamos el atributo userData del actor para guardar la direcci�n del componente
	// l�gico encargado de la f�sica (no es autom�tico)
	controller->getActor()->userData = (void*) component;

	return controller;
}

//--------------------------------------------------------

unsigned CServer::moveController(PxController *controller, const Vector3 &movement, unsigned int msecs)
{
	assert(_scene);

	// TODO: pedir a PhysX que mueva el controller
	// 1. Transformar entre el tipo vector l�gico y el de PhysX
	// 2. Distancia m�nima 0.01
	// 3. PhysX espera el tipo transcurrido en segundos
	// 4. Colisionar contra cualquier objeto (no usar filtros)
	// 5. Devolver flags de colisiones
	PxControllerFilters filters;
	return controller->move( Vector3ToPxVec3(movement), 0.01, msecs/1000.0f, filters );
}

//--------------------------------------------------------

Vector3 CServer::getControllerPosition(const PxCapsuleController *controller)
{
	assert(_scene);

	// TODO: devolver posici�n del controller
	// Transformar entre coordenadas de PhysX y coordenadas l�gicas
	//	- offsetY = altura del controller / 2 + radio del controller
	float offsetY = controller->getHeight() / 2 + controller->getRadius();
	PxExtendedVec3 pos = controller->getPosition();
	pos -= PxExtendedVec3(0, offsetY, 0);
	
	return PxExtendedVec3ToVector3(pos);
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

	// TODO: 1. Inicializar par�metros (origen, direcci�n, distancia,...)

	// TODO: 2. Lanzar rayo al objeto m�s cercano

	// TODO: 3. Devolver entidad l�gica asociada a la entidad f�sica impactada
	
	return NULL;
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosest(const Ray& ray, float maxDist, int group) const 
{
	assert(_scene);

	// Establecer par�metros del rayo
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