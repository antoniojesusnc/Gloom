//---------------------------------------------------------------------------
// ShootMiniGun.h
//---------------------------------------------------------------------------

/**
@file ShootMiniGun.h

Contiene la declaraci�n del componente que implementa el arma minigun.

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/

#ifndef __Logic_ShootMiniGun_H
#define __Logic_ShootMiniGun_H

#include "Logic/Entity/Components/Weapon.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementaci�n por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narv�ez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CShootMiniGun : public IWeapon {
		DEC_FACTORY(CShootMiniGun);

	public:

		/** Constructor por defecto. */
		CShootMiniGun();

		//__________________________________________________________________

		virtual ~CShootMiniGun();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

		virtual void primaryFire();

		virtual void secondaryFire();

		virtual void stopPrimaryFire();

		virtual void stopSecondaryFire();

		virtual void amplifyDamage(unsigned int percentage);

		virtual void reduceCooldown(unsigned int percentage);

	protected:

		virtual void onFixedTick(unsigned int msecs);

		virtual bool canUsePrimaryFire();
		virtual bool canUseSecondaryFire();

		//M�todo que efectua el disparo
		void		 shoot				();

		CEntity*	 fireWeapon			();

		void		 triggerHitMessages	(CEntity* entityHit);

		void		 secondaryShoot		(int iRafagas);

	private:

		unsigned int _defaultPrimaryFireCooldown;
		unsigned int _primaryFireCooldown;
		int _primaryFireCooldownTimer;

		unsigned int _defaultDamage;
		unsigned int _damage;

		/**
		Namespace para los tipos de mensajes de control posibles.
		*/
		bool _pressThenShoot;

		/**
			Contador para ir acumulando el tiempo/ticks que se tiene el bot�n derecho apretado
		*/
		int		_contador;

		/**
			Variable booleana que indica si se est� acumulando tiempo/ticksporque
			se tiene el bot�n derecho pulsado
		*/
		bool	_acumulando;

		/**
			Numero de rafagas que se tienen acumuladas.
			Una rafaga = 10 unidades de contador.
		*/
		int		_iRafagas;

		/**
			Variable booleana que controla si tenemos el click izquierdo pulsado
		*/
		bool	_bLeftClicked;

		/**
			Variable para contar cu�ntas veces se ha pulsado el click izquierdo
		*/
		int		_iContadorLeftClicked;

		/**
			Variable booleana para controlar si se ha mandado ya el mensaje de dispersi�n
		*/
		bool	_bMensajeDispMandado;

		/**
			N�mero m�ximo de r�fagas (balas) que se pueden disparar
		*/
		int		_iMaxRafagas;

		/** Dispersi�n del arma. */
		float _dispersion;

		/** Dispersi�n original del arma. Variable auxiliar para guardar la referencia le�da del mapa.*/
		float _dispersionOriginal;

		/** Distancia de alcance del arma. */
		float _distance;

	}; // class CShootMiniGun

	REG_FACTORY(CShootMiniGun);

} // namespace Logic

#endif // __Logic_Shoot_H
