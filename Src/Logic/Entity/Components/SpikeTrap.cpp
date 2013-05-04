/**
@file SpikeTrap.cpp

Contiene la implementaci�n del componente que controla el movimiento de los pinchos.
 
@see Logic::CSpikeTrap
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Mayo, 2013
*/

#include "SpikeTrap.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageDeactivate.h"


namespace Logic 
{
	IMP_FACTORY(CSpikeTrap);
	
	//---------------------------------------------------------
	
	bool CSpikeTrap::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("coolDown"))
			_coolDownTime = entityInfo->getIntAttribute("coolDown");
		_coolDownTime*=1000;

		_numTrap = entityInfo->getIntAttribute("numTrap");
		_numSpikes = entityInfo->getIntAttribute("numSpikes");
		_spikes = new CEntity*[_numSpikes];
		_velocitySpikes = entityInfo->getIntAttribute("velocitySpikes");
		_directionSpikes = entityInfo->getVector3Attribute("directionSpikes");
		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CSpikeTrap::onActivate()
	{
		std::shared_ptr<CMessageDeactivate> deactivateMsg = std::make_shared<CMessageDeactivate>();
		// Leer los parametros que toquen para los proyectiles
		std::stringstream numTrap;
		numTrap << _numTrap;
		std::string nTrap = numTrap.str();
		//Cogemos los pinchos asociados a esta trampa 
		//Hay que desactivarlos en el onStart si es que esta bien o hacer la creacion de entidades de forma desactivada grafica/fisicamente
		for(int i=1; i<=_numSpikes; i++){
			std::stringstream numI;
			numI << i;
			std::string nI = numI.str();
			_spikes[i-1] = CServer::getSingletonPtr()->getMap()->getEntityByName("Pincho"+nTrap+nI);
			_spikes[i-1]->emitMessage(deactivateMsg);
		}
		_isRespawning=false;
		_timer=0;

	} // activate
	//---------------------------------------------------------


	bool CSpikeTrap::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
	} // accept
	
	//---------------------------------------------------------

	void CSpikeTrap::process(const std::shared_ptr<CMessage>& message) {
		
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				if(!_isRespawning){
					std::cout << "ACTIVAMOS LA TRAMPA" << std::endl;
					//Creamos el mensaje de fuerza para los pinchos
					std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
					forceMsg->setForce(_directionSpikes* _velocitySpikes, Physics::ForceMode::eFORCE );
					forceMsg->setGravity(false);
					//Activacion y fuerza
					std::shared_ptr<CMessageActivate> activateMsg = std::make_shared<CMessageActivate>();
					for(int i=0; i<_numSpikes; i++){
						_spikes[i]->emitMessage(activateMsg);
						_spikes[i]->emitMessage(forceMsg);
					}
					_isRespawning=true;
				}
				break;
			}
		}

	} // process
	//----------------------------------------------------------


	void CSpikeTrap::onTick(unsigned int msecs){
		// Si la trampa esta en la fase de spawn
		if(_isRespawning) {
			_timer += msecs;
			if(_timer >= _coolDownTime) {
				_isRespawning = false;
				_timer = 0;
			}
		}
	} // onTick
	//----------------------------------------------------------

} // namespace Logic


