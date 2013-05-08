/**
@file PhysicController.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Francisco Aisa Garc�a
@date Marzo, 2013.
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageCealing.h"
#include "Logic/Messages/MessageSide.h"

#include "AvatarController.h"

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CPhysicController);

//________________________________________________________________________

CPhysicController::CPhysicController() {
	// _controller <- Su valor se auto inicializa
	// en el proyecto de f�sica
}

//________________________________________________________________________

CPhysicController::~CPhysicController() {
	// El destructor de _controller se auto ejecuta
} 

//________________________________________________________________________

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
	// Invocar al m�todo de la clase padre
	if( !IComponent::spawn(entity,map,entityInfo) ) return false;

	// Crear el character controller asociado al componente
	createController(entityInfo);

	return true;
}

//________________________________________________________________________

/*bool CPhysicController::accept(CMessage *message) {
	return message->getMessageType() == Message::AVATAR_WALK;
}

//________________________________________________________________________

void CPhysicController::process(const std::shared_ptr<CMessage>& message) {
	switch( message->getMessageType() ) {
		case Message::AVATAR_WALK: {
			// Anotamos el vector de desplazamiento para usarlo posteriormente en 
			// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
			// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
			_movement = std::static_pointer_cast<CMessageAvatarWalk>(message)->getDirection();
		
			break;
		}
	}
}

//________________________________________________________________________

void CPhysicController::onTick(unsigned int msecs) {
	// Sino hay movimiento no hacemos nada
	if(_movement == Vector3::ZERO) return;

	// Movemos el character controller
	moveController(_movement, msecs);
}*/

//________________________________________________________________________

void  CPhysicController::setPhysicPosition (const Vector3 &position){
	//Teletransportamos al player y ponemos la logica en el mismo momento(sino ocurrir�an teletransportaciones gr�ficas)
	_controller.setPosition(position);
	_entity->setPosition( _controller.getPosition() );
}

//________________________________________________________________________

void CPhysicController::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
	// Leer el grupo de colisi�n (por defecto grupo 0)
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Comprobamos los grupos con los que esta entidad deberia colisionar
	if (entityInfo->hasAttribute("physic_groupList")) {
		std::istringstream groupListString(entityInfo->getStringAttribute("physic_groupList"));

		// Para cada cadena entre comas...
		do {
			std::string groupNumber;
			std::getline(groupListString, groupNumber, ','); // linea entre delimitadores
				
			std::istringstream str(groupNumber);     // wrappeamos cadena como Input Stream
			do {									// Le quitamos los espacios...
				std::getline(str, groupNumber, ' ');  // linea entre espacios
			} while (groupNumber.size() == 0 && !str.eof());

			groupList.push_back( atoi(groupNumber.c_str()) );
		} while (!groupListString.eof());
	}
}

//________________________________________________________________________

void CPhysicController::createController(const Map::CEntity *entityInfo) {
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

	// Leer la informacion sobre el grupo de colision de la capsula
	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Inicializar el controller de tipo capsula
	_controller.load(position, radius, height, group, groupList, this);
} 

//________________________________________________________________________

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter) {

}

//________________________________________________________________________

void CPhysicController::onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) {
	// Implementar la funcionalidad que corresponda en IPhysics, aunque
	// si nos vemos forzados a hacer este tipo de �apas es que algo estamos
	// haciendo mal. Hay que ser mas elegantes for the win.
}

//________________________________________________________________________

void CPhysicController::onContact (IPhysics *otherComponent, bool enter) {
	//std::cout<< "contacto entre kinematics en oncontact" << std::endl;
}

//________________________________________________________________________

void CPhysicController::deactivateSimulation() {
	_controller.deactivateSimulation();
}

//________________________________________________________________________

void CPhysicController::activateSimulation() {
	_controller.activateSimulation();
}

//________________________________________________________________________

unsigned CPhysicController::move(const Vector3& movement, unsigned int msecs) {
	// Intentamos mover el controller a la posici�n que se haya calculado
	// desde avatarController
	unsigned collisionFlags = _controller.move(movement, msecs);

	// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
	// informaci�n proporcionada por el motor de f�sica	
	_entity->setPosition( _controller.getPosition() );

	return collisionFlags;
}
