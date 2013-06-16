//---------------------------------------------------------------------------
// HardDeathFeedback.h
//---------------------------------------------------------------------------

/**
@file HardDeathFeedback.h

Contiene la declaraci�n de la cabecera del componente
de hechizo del hardDeath.

@see Logic::CCoolDownFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_HardDeathFeedback_H
#define __Logic_HardDeathFeedback_H

#include "Logic/Entity/Components/SpellFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class CHardDeathFeedback : public ISpellFeedback {
		DEC_FACTORY(CHardDeathFeedback);

	public:

		/** Constructor por defecto. */
		CHardDeathFeedback();

		//__________________________________________________________________

		virtual ~CHardDeathFeedback();

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

	REG_FACTORY(CHardDeathFeedback);

} // namespace Logic

#endif // __Logic_CHardDeathFeedback_H