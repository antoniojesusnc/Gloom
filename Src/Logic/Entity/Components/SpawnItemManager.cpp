/**
@file SpawnItemManager.cpp

@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#include "SpawnItemManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/PhysicEntity.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"

namespace Logic 
{
	IMP_FACTORY(CSpawnItemManager);
	
	//---------------------------------------------------------
	void CSpawnItemManager::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_isRespawning) {
			_timer += msecs;

			std::cout << "El tiempo transcurrido desde que se capturo es " << _timer << std::endl;

			if(_timer >= _respawnTime) {
				_isRespawning = false;

				

				// Activar entidad grafica
				_entity->getComponent<CGraphics>("CGraphics")->activate();

				// Activar entidad fisica
				_entity->getComponent<CPhysicEntity>("CPhysicEntity")->activate();
			}
		}
	} // tick

	bool CSpawnItemManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("id")) {
			_id = entityInfo->getStringAttribute("id");

			if(_id == "weapon" || _id == "ammo") {
				if(entityInfo->hasAttribute("weaponType")) {
					_weaponType = entityInfo->getStringAttribute("weaponType");
				}
			}
		}
		if(entityInfo->hasAttribute("reward")) {
			_reward = entityInfo->getIntAttribute("reward");
		}
		if(entityInfo->hasAttribute("respawnTime")) {
			_respawnTime = entityInfo->getFloatAttribute("respawnTime");
			_respawnTime *= 1000; // Convertimos en segundos
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSpawnItemManager::accept(CMessage *message)
	{
		return (message->getMessageType() == Message::TOUCHED);
	} // accept
	
	//---------------------------------------------------------

	void CSpawnItemManager::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
			{
				itemGrabbed( ((CMessageTouched*)message)->getEntity() );
			}
		}

	} // process

	void CSpawnItemManager::itemGrabbed(CEntity* actor) {
		// Desactivar entidad grafica
		_entity->getComponent<CGraphics>("CGraphics")->deactivate();

		// Desactivar entidad fisica
		_entity->getComponent<CPhysicEntity>("CPhysicEntity")->deactivate();
		
		// Mandar el mensaje que corresponda a la entidad actuadora
		// en funcion del item que se haya cogido (comprobando el id)
		if(_id == "orb") {
			CMessageAddLife* m = new CMessageAddLife();
			m->setAddLife(_reward);
			actor->emitMessage(m);
		}
		else if(_id == "armor") {
			CMessageAddShield* m = new CMessageAddShield();
			m->setAddShield(_reward);
			actor->emitMessage(m);
		}
		else if(_id == "ammo") {
			// Mandar un mensaje con el _weaponType
		}
		else if(_id == "weapon") {
			// Mandar un mensaje con el _weaponType
		}

		// Arrancar el timer
		_timer = 0;
		_isRespawning = true;
	}

} // namespace Logic

