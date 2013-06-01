//---------------------------------------------------------------------------
// AltShoot.h
//---------------------------------------------------------------------------

/**
@file AltShoot.h

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_AltShoot_H
#define __Logic_AltShoot_H

#include "Logic/Entity/Component.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup


	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CAltShoot : public IComponent {
	public:

		CAltShoot(const std::string& weaponName);

		virtual ~CAltShoot();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void process(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		// @abstract
		virtual void primaryFire() = 0;
		// @abstract
		virtual void secondaryFire() = 0;

		virtual void stopPrimaryFire() { /* Por defecto nada */ }

		virtual void stopSecondaryFire() { /* Por defecto nada */ }

	protected:

		virtual void onTick(unsigned int msecs);

		virtual void onAvailable();

	private:

		bool canUsePrimaryFire();

		bool canUseSecondaryFire();
		
		// Municion 

		/** Cuanta munici�n puede llevar este arma como m�ximo. */
		unsigned int _maxAmmo;
		
		/** Cuanta munici�n tenemos actualmente en este arma. */
		unsigned int _currentAmmo;
		
		/** Cuantas balas se gastan por disparo primario. */
		unsigned int _defaultAmmoSpentPerPrimaryShot;
		
		/** 
		Cuantas balas se gastan como m�ximo al cargar hasta el l�mite el disparo
		primario. Solo v�lido para armas toggle.
		*/
		unsigned int _maxAmmoSpentPerPrimaryShot;

		/** Tiempo de carga m�ximo para el disparo primario. */
		float _primaryFireMaxLoadTime;
		
		/** Cuantos disparos se hacen al activar el disparo primario. */
		unsigned int _shotsPerPrimaryFire;

		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _defaultAmmoSpentPerSecondaryShot;

		/** 
		Cuantas balas se gastan como m�ximo al cargar hasta el l�mite el disparo
		secundario. Solo v�lido para armas toggle.
		*/
		unsigned int _maxAmmoSpentPerSecondaryShot;

		/** Tiempo de carga m�ximo para el disparo secundario. */
		float _secondaryFireMaxLoadTime;

		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _shotsPerSecondaryFire;


		// Cooldowns

		/** Cooldown del disparo primario. */
		float _primaryFireCooldown;

		/** Cooldown del disparo secundario */
		float _secondaryFireCooldown;


		// Para saber si los disparos son toggle o no

		bool _primaryFireIsToggle;

		bool _secondaryFireIsToggle;

		std::string _weaponName;

	}; // class CAltShoot

} // namespace Logic

#endif // __Logic_AltShoot_H
