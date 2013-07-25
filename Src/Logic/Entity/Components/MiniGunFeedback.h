//---------------------------------------------------------------------------
// MiniGunFeedback.h
//---------------------------------------------------------------------------

/**
@file MiniGunFeedback.h

Contiene la declaraci�n de la cabecera del componente
de disparo de la cabra.

@see Logic::CMiniGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_MiniGunFeedback_H
#define __Logic_MiniGunFeedback_H

#include "Logic/Entity/Components/WeaponFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class CMiniGunFeedback : public IWeaponFeedback {
		DEC_FACTORY(CMiniGunFeedback);

	public:

		/** Constructor por defecto. */
		CMiniGunFeedback();

		//__________________________________________________________________

		virtual ~CMiniGunFeedback();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		// El propio shoot deberia encargarse de llamar a lo que toque teniendo
		// en cuenta la municion y las demas historias
		virtual void primaryFire();

		//__________________________________________________________________

		virtual void stopPrimaryFire();

		//__________________________________________________________________

		virtual void secondaryFire();

		//__________________________________________________________________

		virtual void stopSecondaryFire();

		//__________________________________________________________________
		virtual void onFixedTick(unsigned int msecs);

	protected:

		void flashAnim();

		unsigned int _defaultPrimaryFireCooldown;
		unsigned int _primaryFireCooldown;
		int _primaryFireCooldownTimer;

		bool _primaryFireIsActive;

		/** Dispersi�n del arma. */
		float _dispersion;

		/** Dispersi�n original del arma. Variable auxiliar para guardar la referencia le�da del mapa.*/
		float _dispersionOriginal;

		/**
		Variable para contar cu�ntas veces se ha pulsado el click izquierdo
		*/
		int	_iContadorLeftClicked;

		/**
		Variable booleana para controlar si se ha mandado ya el mensaje de dispersi�n
		*/
		bool _bMensajeDispMandado;
		bool _firingRound;
	};

	REG_FACTORY(CMiniGunFeedback);

} // namespace Logic

#endif // __Logic_MiniGunFeedback_H