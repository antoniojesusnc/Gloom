//---------------------------------------------------------------------------
// CoolDownFeedback.h
//---------------------------------------------------------------------------

/**
@file CoolDownFeedback.h

Contiene la declaraci�n de la cabecera del componente
de disparo de la cabra.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_CoolDownFeedback_H
#define __Logic_CoolDownFeedback_H

#include "Logic/Entity/Components/SpellFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class CCoolDownFeedback : public ISpellFeedback {
		DEC_FACTORY(CCoolDownFeedback);

	public:

		/** Constructor por defecto. */
		CCoolDownFeedback();

		//__________________________________________________________________

		virtual ~CCoolDownFeedback();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		M�todo virtual que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el spellCast
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopSpell();

		//__________________________________________________________________
	};

	REG_FACTORY(CCoolDownFeedback);

} // namespace Logic

#endif // __Logic_CoolDownFeedback_H