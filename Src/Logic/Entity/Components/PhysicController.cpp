/**
@file PhysicController.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageCealing.h"
#include "Logic/Messages/MessageSide.h"

#include <PxPhysicsAPI.h>


using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(), _controller(NULL), 
								       _movement(0,0,0), _falling(false)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	_server = NULL;
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);

	// Seteo de _falling a false para que se envie el primer mensaje de actualizacion
	_falling=false;

	return true;
}

//---------------------------------------------------------

bool CPhysicController::accept(CMessage *message)
{
	return message->getMessageType() == Message::AVATAR_WALK ||
		message->getMessageType() == Message::SET_PHYSIC_POSITION;
} 

//---------------------------------------------------------

void CPhysicController::process(CMessage *message)
{
	switch(message->getMessageType())
	{
	case Message::AVATAR_WALK:
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
		_movement = ((CMessageAvatarWalk*)message)->getDirection();
		break;
	case Message::SET_PHYSIC_POSITION:
		setPhysicPosition(((CMessageSetPhysicPosition*)message)->getPosition());
		break;
	}

}


//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al m�todo de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
	// informaci�n proporcionada por el motor de f�sica	
	_entity->setPosition(_server->getControllerPosition(_controller));

	// Intentamos mover el controller a la posici�n recibida en el �ltimo mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = _server->moveController(_controller, _movement, msecs);

	//Si tocamos con el techo lo notificamos
	if((flags & PxControllerFlag::eCOLLISION_UP)){
		Logic::CMessageCealing *em=new Logic::CMessageCealing();
		_entity->emitMessage(em);
	}
	//Si tocamos el lateral tenemos que parar la inercia de la direccion
	if((flags & PxControllerFlag::eCOLLISION_SIDES)){
		Logic::CMessageSide *side=new Logic::CMessageSide();
		_entity->emitMessage(side);
	}
	//Si hay cambio de estado en el flag de tocar suelo
	if(_falling != !(flags & PxControllerFlag::eCOLLISION_DOWN)){
		// Actualizamos el flag que indica si estamos cayendo
		_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);
		//Mandamos un mensaje que dir� si hay collision con el suelo para la l�gica
		Logic::CMessageCollisionDown *m=new Logic::CMessageCollisionDown();
		m->setCollisionDown(_falling);
		_entity->emitMessage(m);
	}
	_movement = Vector3::ZERO;

}

//---------------------------------------------------------

void  CPhysicController::setPhysicPosition (const Vector3 &position){
	_server->setControllerPosition(_controller,position);
}


//---------------------------------------------------------

PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posici�n de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulaci�n.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisi�n del controller. Por ahora s�lo admitimos c�psulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la c�psula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la c�psula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Crear el controller de tipo c�psula
	return _server->createCapsuleController(position, radius, height, this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad est�tica no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor().isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinem�tica no hacemos nada
	if (_server->isKinematic(actor))
		return;

	// Aplicar una fuerza a la entidad en la direcci�n del movimiento
	//actor->addForce(hit.dir * hit.length * 1000.0f);
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}

//---------------------------------------------------------



