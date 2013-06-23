//---------------------------------------------------------------------------
// ComeBackAmmo.h
//---------------------------------------------------------------------------

/**
@file ComeBackAmmo.h

@see Logic::CComeBackAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ComeBackAmmo_H
#define __Logic_ComeBackAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CComeBackAmmo : public ISpellAmmo {
		DEC_FACTORY(CComeBackAmmo);
	public:

		/** Constructor por defecto. */
		CComeBackAmmo();

		//__________________________________________________________________

		virtual ~CComeBackAmmo();

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

		float getCoolDown(){return _cooldown;}
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

		

		int _cooldownTimer;
		int _durationTimer;

		unsigned int _distanceComeBack;

		Vector3 _comeBackPosition;
		Quaternion _comeBackOrientation;
		
	};

	REG_FACTORY(CComeBackAmmo);

} // namespace Logic

#endif // __Logic_ComeBackAmmo_H