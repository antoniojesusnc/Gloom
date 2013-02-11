//---------------------------------------------------------------------------
// SinglePlayerState.h
//---------------------------------------------------------------------------

/**
@file SinglePlayerState.h

Contiene la declaraci�n del estado de juego.

@see Application::CGameState
@see Application::SinglePlayerState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_SinglePlayerState_H
#define __Application_SinglePlayerState_H

#include "GameState.h"

namespace Application {
	/**
	@ingroup applicationGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/
	class CSinglePlayerState : public CGameState {
	public:
		/** Constructor de la clase */
		CSinglePlayerState(CBaseApplication *app) : CGameState(app) {}
	}; // CSinglePlayerState

} // namespace Application

#endif //  __Application_GameState_H
