//---------------------------------------------------------------------------
// SinglePlayerState.h
//---------------------------------------------------------------------------

/**
@file SinglePlayerState.h

Contiene la declaraci�n del estado de juego.

@see Application::CGameState
@see Application::CSinglePlayerState

@author Francisco Aisa Garc�a
@date Febrero, 2013
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
		
		/** 
		Constructor por parametro.

		@param app Aplicacion que se encarga de lanzar los estados.
		*/
		CSinglePlayerState(CBaseApplication *app) : CGameState(app) {}
	}; // CSinglePlayerState

} // namespace Application

#endif //  __Application_GameState_H
