//---------------------------------------------------------------------------
// AmplifyDamageAmmo.h
//---------------------------------------------------------------------------

/**
@file AmplifyDamageAmmo.h

@see Logic::CAmplifyDamageAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_AmplifyDamageAmmo_H
#define __Logic_AmplifyDamageAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CAmplifyDamageAmmo : public ISpellAmmo {
		DEC_FACTORY(CAmplifyDamageAmmo);
	public:

		/** Constructor por defecto. */
		CAmplifyDamageAmmo();

		//__________________________________________________________________

		virtual ~CAmplifyDamageAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


		virtual void reduceCooldown(unsigned int percentage);

		//__________________________________________________________________

		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopSpell();

		virtual void addAmmo();
	protected:

		virtual bool canUseSpell();


		virtual void onActivate();

		virtual void onWake();

		virtual void onTick(unsigned int msecs);

	private:

		//std::string _shootAudio;

		unsigned int _defaultCooldown;
		unsigned int _cooldown;

		unsigned int  _duration;
		
		unsigned int _maxAmmo;
		unsigned int _ammoPerPull;
		unsigned int _currentAmmo;

		bool _spellIsActive;

		int _cooldownTimer;
		int _durationTimer;
		
	};

	REG_FACTORY(CAmplifyDamageAmmo);

} // namespace Logic

#endif // __Logic_ComeBackAmmo_H