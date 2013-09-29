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
#include "Logic/Maps/WorldState.h"

#include "WeaponAmmo.h"
/*#include "ShootShotGunAmmo.h"
#include "ShootSniperAmmo.h"
#include "ShootMiniGunAmmo.h"
#include "ShootSoulReaperAmmo.h"
#include "IronHellGoatAmmo.h"*/

#include "ShotGunAmmo.h"
#include "SniperAmmo.h"
#include "MiniGunAmmo.h"
#include "SoulReaperAmmo.h"
#include "IronHellGoatAmmo.h"

#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"

#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"

#include "Logic/Messages/MessageDamageAmplifier.h"
#include "Logic/Messages/MessageReducedCooldown.h"


namespace Logic 
{
	IMP_FACTORY(CWeaponsManager);

	//---------------------------------------------------------
	
	CWeaponsManager::CWeaponsManager() : _currentWeapon(0), _cooldownTimer(0), _dmgAmpTimer(0) {
		
	}

	//---------------------------------------------------------

	bool CWeaponsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		// Inicializamos el vector de armas.
		// De momento no tiene mucho sentido que comprobemos el n�mero de armas que hay
		// por que luego usamos el getComponent a pi�on.
		_weaponry.resize(WeaponType::eSIZE);

		// Rellenamos el vector con los punteros a los componentes correspondientes
		_weaponry[WeaponType::eSOUL_REAPER].second = _entity->getComponent<Logic::CSoulReaperAmmo>("CSoulReaperAmmo");
		_weaponry[WeaponType::eSNIPER].second = _entity->getComponent<Logic::CSniperAmmo>("CSniperAmmo");
		_weaponry[WeaponType::eSHOTGUN].second = _entity->getComponent<Logic::CShotGunAmmo>("CShotGunAmmo");
		_weaponry[WeaponType::eMINIGUN].second = _entity->getComponent<Logic::CMiniGunAmmo>("CMiniGunAmmo");
		_weaponry[WeaponType::eIRON_HELL_GOAT].second = _entity->getComponent<Logic::CIronHellGoatAmmo>("CIronHellGoatAmmo");

		/*
		// Por defecto la primera arma est� activada y equipada (es el arma 0).
		_weaponry[eSOUL_REAPER].first = true;
		_weaponry[eSOUL_REAPER].second->stayAvailable();
		_weaponry[eSOUL_REAPER].second->inUse(true);

		
		// El resto de las armas est�n desactivadas, ya que no las tenemos
		for(unsigned int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false;
			_weaponry[i].second->stayBusy();
			_weaponry[i].second->inUse(false);
		}
		*/

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CWeaponsManager::onActivate() {
		
		// El resto de las armas est�n desactivadas, ya que no las tenemos
		for(unsigned int i = 1; i < _weaponry.size(); ++i) {
			_weaponry[i].first = false; // Por si acaso habian sido activadas anteriormente
			_weaponry[i].second->stayBusy();
			_weaponry[i].second->resetAmmo();

		}

		// El arma actual tiene que ser el soulReaper, que
		// es la �nica que tenemos de primeras
		_currentWeapon=WeaponType::eSOUL_REAPER;
		// Por defecto la primera arma est� activada y equipadda
		_weaponry[WeaponType::eSOUL_REAPER].first = true;
		_weaponry[WeaponType::eSOUL_REAPER].second->stayAvailable();
	} // onActivate
	//---------------------------------------------------------

	void CWeaponsManager::onDeactivate(){
		/*
		//reset de armas y balas
		for(unsigned int i = 0; i<_weaponry.size();++i){
			_weaponry[i].first = false;
			_weaponry[i].second->resetAmmo();
			_weaponry[i].second->inUse(false);
		}
		*/
		
	}// onDeactivate
	//---------------------------------------------------------

	bool CWeaponsManager::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::CHANGE_WEAPON 
			|| msgType == Message::ADD_AMMO
			|| msgType == Message::ADD_WEAPON
			|| msgType == Message::REDUCED_COOLDOWN
			|| msgType == Message::DAMAGE_AMPLIFIER;
	} // accept
	
	//---------------------------------------------------------

	void CWeaponsManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON: {
				std::shared_ptr<CMessageChangeWeapon> changeWeaponMsg = std::static_pointer_cast<CMessageChangeWeapon>(message);
				int iWeapon = changeWeaponMsg->getWeapon();

				//Si iWeapon no es scroll de rueda de rat�n hacia adelante o hacia atr�s, 
				//asignamos directamente el arma con el �ndice recibido
				if ((iWeapon != 100) && (iWeapon != -100)){
					changeWeapon(iWeapon);
				}
				else{
					//Obtenemos el �ndice del arma nueva al que se va a cambiar por el scroll
					iWeapon = selectScrollWeapon(iWeapon);
					if (iWeapon != -1)
						changeWeapon(iWeapon); //Si hemos obtenido arma, se la asignamos
				}
				break;
			}
			case Message::ADD_AMMO: {
				
				std::shared_ptr<CMessageAddAmmo> addAmmoMsg = std::static_pointer_cast<CMessageAddAmmo>(message);
				unsigned int weaponIndex = addAmmoMsg->getAddWeapon();
				_weaponry[weaponIndex].second->addAmmo(weaponIndex, addAmmoMsg->getAddAmmo(), _weaponry[weaponIndex].first);
				break;
			}
			case Message::ADD_WEAPON: {
				std::shared_ptr<CMessageAddWeapon> addWeaponMsg = std::static_pointer_cast<CMessageAddWeapon>(message);
				addWeapon( addWeaponMsg->getAddAmmo(), addWeaponMsg->getAddWeapon() );
				break;
			}
			case Message::REDUCED_COOLDOWN: {
				std::shared_ptr<CMessageReducedCooldown> cooldownMsg = std::static_pointer_cast<CMessageReducedCooldown>(message);
				reduceCooldowns( cooldownMsg->getPercentCooldown() );
				_cooldownTimer = cooldownMsg->getDuration();
				break;
			}
			case Message::DAMAGE_AMPLIFIER: {
				std::shared_ptr<CMessageDamageAmplifier> damageAmplifierMsg = std::static_pointer_cast<CMessageDamageAmplifier>(message);
				amplifyDamage( damageAmplifierMsg->getPercentDamage() );
				_dmgAmpTimer = damageAmplifierMsg->getDuration();
				break;
			}
		}

	} // process

	//---------------------------------------------------------

	void CWeaponsManager::onTick(unsigned int msecs) {
		if(_cooldownTimer != 0) {
			_cooldownTimer -= msecs;

			if(_cooldownTimer <= 0) {
				_cooldownTimer = 0;
				reduceCooldowns(0);
			}
		}

		if(_dmgAmpTimer != 0) {
			_dmgAmpTimer -= msecs;

			if(_dmgAmpTimer <= 0) {
				_dmgAmpTimer = 0;
				amplifyDamage(0);
			}
		}
	}
	
	//---------------------------------------------------------

	void CWeaponsManager::changeWeapon(unsigned char newWeapon){
		
		//printf("\n%d\t%d",_currentWeapon, newWeapon);
		if(newWeapon >= WeaponType::eSIZE){
			return;
		}
		if( _weaponry[newWeapon].first && (newWeapon != _currentWeapon) && ( _weaponry[newWeapon].second->getAmmo() > 0 || newWeapon == WeaponType::eSOUL_REAPER ) )
		{
			// Indicamos que el arma actual ya no est� equipada
			// Desactivamos el componente Shoot del arma actual
			// e indicamos que ya no est� equipada
//_weaponry[_currentWeapon].second->inUse(false);
			_weaponry[_currentWeapon].second->stayBusy();

			// Activamos el componente del nuevo arma que vamos
			// a equipar e indicamos que el arma est� equipada
			_weaponry[newWeapon].second->stayAvailable();
//_weaponry[newWeapon].second->inUse(true);
			
			// Actualizamo el indice de arma
			_currentWeapon = newWeapon;

		}
		
	}

	//---------------------------------------------------------

	void CWeaponsManager::amplifyDamage(int percentage) {
		// Amplificamos el da�o de todas las armas en base al porcentaje dado
		for(unsigned int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->amplifyDamage(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas las armas en base al porcentaje dado
		for(unsigned int i = 0; i < _weaponry.size(); ++i) {
			_weaponry[i].second->reduceCooldown(percentage);
		}
	}

	//---------------------------------------------------------

	void CWeaponsManager::addWeapon(int ammo, int weaponIndex){
		// Si el arma dada no la teniamos, indicamos que ahora la tenemos
		if(weaponIndex < WeaponType::eSIZE && !_weaponry[weaponIndex].first)
			_weaponry[weaponIndex].first = true;

		// Activamos el componente pero indicamos que
		// no es el arma equipada.
		if(_currentWeapon != weaponIndex){
			_weaponry[weaponIndex].second->stayBusy();
//_weaponry[weaponIndex].second->inUse( false );
		}
		/*
		else{
			//_weaponry[weaponIndex].second->stayAvailable();
			//_weaponry[weaponIndex].second->inUse( true );
		}
		*/
		
		// El arma estara en uso si es la actual, si no estara sin uso
		
		_weaponry[weaponIndex].second->addAmmo(weaponIndex, ammo, _weaponry[weaponIndex].first);

		/*
		// Enviamos un mensaje de actualizacion del hud
		std::shared_ptr<CMessageHudAmmo> *m=std::make_shared<CMessageHudAmmo>();
		m->setWeapon(weaponIndex);
		m->setAmmo(ammo);//No es necesario esto, ya que solo actualizare el hud como que puedo coger el arma pero no mostrara sus balas(en este caso concreto)
		_entity->emitMessage(m);
		*/
		//si llevabamos la melee, cambiamos de melee al arma que acabamos de coger
		if(_currentWeapon == WeaponType::eSOUL_REAPER){
			changeWeapon(weaponIndex);
		}
	}

	//---------------------------------------------------------

	int CWeaponsManager::selectScrollWeapon(int iWeapon) 
	{
		if (iWeapon == 100) //Armas siguientes
		{
			//Recorremos todas las armas del inventario desde la actual hasta el final
			for (int i = _currentWeapon + 1; i < _weaponry.size(); ++i)
			{
				//Comprobamos si en ese �ndice tenemos arma
				if (_weaponry[i].first)
					return i;
			}
		}
		else //iWeapon == -100 Armas anteriores
		{
			//Recorremos todas las armas del inventario desde la actual hasta el principio
			for (int i = _currentWeapon - 1; i >= 0; --i)
			{
				//Comprobamos si en ese �ndice tenemos arma
				if (_weaponry[i].first)
					return i;
			}
		}

		return -1; //No hemos obtenido arma
	}//selectScrollWeapon

} // namespace Logic

