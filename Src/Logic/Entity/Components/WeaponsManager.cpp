/**
@file Shoot.cpp

Contiene la implementaci�n del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "WeaponsManager.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"

#include "Logic/Entity/Components/ShootShotGun.h"
#include "Logic/Entity/Components/ShootMiniGun.h"


#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"




namespace Logic 
{
	IMP_FACTORY(CWeaponsManager);
	
	//---------------------------------------------------------

	bool CWeaponsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("numWeapons")){
			_numWeapons = entityInfo->getIntAttribute("numWeapons");

			_weapons = new bool[_numWeapons];
			for(int i = 0; i<_numWeapons;++i){
				_weapons[i] = false;
				desactivateComponent(i);
			}

			/*la 1� arama siempre estara a true*/
			_weapons[0] = true;
			//activateComponent(_actualWeapon);

			_weapons[1] = true;

			printf("\n\n mi actual weapon es.....%d", _actualWeapon);
		}
		return true;

	} // spawn
	//---------------------------------------------------------

	void CWeaponsManager::activate()
	{
		IComponent::activate();

		for(int i = 0; i<_numWeapons;++i){
				desactivateComponent(i);
			}

			/*la 1� arama siempre estara a true*/
			activateComponent(_actualWeapon);

		//return true;

	} // activate

	bool CWeaponsManager::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CHANGE_WEAPON;
	} // accept
	//---------------------------------------------------------


	void CWeaponsManager::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			case Message::CHANGE_WEAPON:
					changeWeapon( ((CMessageChangeWeapon*)message)->getWeapon() );
			break;
		}

	} // process
	//---------------------------------------------------------

	void CWeaponsManager::changeWeapon(unsigned char newWeapon){
		
		//printf("\n%d\t%d",_actualWeapon, newWeapon);
		if(newWeapon >= _numWeapons){
			return;
		}
		if(_weapons[newWeapon] && (newWeapon != _actualWeapon))
		{

			desactivateComponent(_actualWeapon);
			_actualWeapon = newWeapon;
			activateComponent(newWeapon);
			Logic::CMessageChangeWeaponGraphics *m=new Logic::CMessageChangeWeaponGraphics();
			m->setWeapon(_actualWeapon);
			_entity->emitMessage(m);
		}
		
	}

	void CWeaponsManager::desactivateComponent(unsigned char weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->deactivate();
			break;
		case 1:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;

		/*
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;
		*/
		}
	}

	void CWeaponsManager::activateComponent(unsigned char weapon){
		switch(weapon){
		case 0:
			_entity->getComponent<CShootMiniGun>("CShootMiniGun")->activate();
			break;
		case 1:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->activate();
			break;

		/*
		case 2:
			_entity->getComponent<CShootShotGun>("CShootShotGun")->deactivate();
			break;
		*/
		}
	}

} // namespace Logic

