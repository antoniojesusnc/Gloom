/**
@file PhysicEntity.h

Contiene la implementaci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IPhysics(), _actor(NULL), _movement(0,0,0)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicEntity::~CPhysicEntity() 
{
	if (_actor) {
		_server->destroyActor(_actor);
		_actor = NULL;
	}

	_server = NULL;
} 

////---------------------------------------------------------

bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el objeto f�sico asociado al componente
	_actor = createActor(entityInfo);

	return true;
} 

//---------------------------------------------------------

bool CPhysicEntity::accept(const TMessage &message)
{
	// TODO: aceptar mensajes de tipo KINEMATIC_MOVE
	return false;
}

//---------------------------------------------------------

void CPhysicEntity::process(const TMessage &message)
{
	switch(message._type) {
	// TODO: procesar mensaje de tipo KINEMATIC_MOVE
	// Acumular el vector de desplazamiento recibido en el mensaje en el 
	// atributo privado _movement para posteriormente utilizarlo en el
	// m�todo tick.

	}
}

//---------------------------------------------------------

void CPhysicEntity::tick(unsigned int msecs) 
{
	// Invocar al m�todo de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Si es una entidad est�tica no hacemos nada
	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	if (!dinActor) 
		return;

	// TODO: actualizar la posici�n y orientaci�n de la entidad l�gica
	// usando la informaci�n proporcionada por el motor de f�sica
	Matrix4 transform = _server->getActorTransform(_actor);
	_entity->setTransform(transform);

	// TODO: mover objetos cinem�ticos de acuerdo a la l�gica
	// 1. Comprobar si el componente representa a un objeto cinem�tico usando
	//    la interfaz del servidor de f�sica
	// 2. Mover el objeto usando la interfaz del servidor de f�sica
	// 3. Poner el atributo _movement a cero para el siguiente tick
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return NULL;
}

PxRigidStatic* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
{
	// La posici�n de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// TODO: Leer el vector normal al plano del fichero de mapa
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	// TODO: leer atributo de grupo de colisi�n (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");
 
	// TODO: Usar la interfaz del servidor de f�sica para crear un plano est�tico
	// Pasar como componente this
	return _server->createPlane(point, normal, group, this);
}

PxRigidActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
{
	// Leer la posici�n de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: est�ticos, din�mico o cinem�tico
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

	// TODO: Leer la forma (shape) 
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");

	// TODO: Leer atributo trigger del mapa (por defecto no es un trigger)
	bool trigger = false;

	// TODO: leer atributo de grupo de colisi�n (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	if (physicType == "static") {
		if (physicShape == "box") {
			// TODO: leer las dimensiones de la caja
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja est�tica
			return _server->createStaticBox(position, physicDimensions, trigger, group, this);
		}

	} else {
		// TODO: Leer la masa (por defecto 0)
		float mass = 0;
		if (entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");
		
		// Leer si se trata de un actor cinem�tico
		bool kinematic = (physicType == "kinematic");

		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// TODO: Crear y devolver una caja din�mica usando el servidor de f�sica
			// Pasar como componente this
			return _server->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, this);
		}
	}

	return NULL;
}

PxRigidActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	// Leer el grupo de colisi�n (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// TODO: crear el actor a partir del fichero RepX usando el servidor de f�sica
	return false;
}


void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
{
	// TODO: notificar que entramos o salimos de un trigger
	// Construir un mensaje de tipo TOUCHED si entramos en el trigger 
	// o UNTOUCHED si salimos del trigger y enviarlo al resto de 
	// componentes de la entidad.

	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 
	TMessage msg;
	if (enter) {
		msg._type = Message::TOUCHED;
	} else {
		msg._type = Message::UNTOUCHED;
	}
	msg._entity = otherComponent->getEntity();

	_entity->emitMessage(msg);
}

