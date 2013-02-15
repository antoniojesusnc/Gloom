//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchClientState.h
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchClientState.h

Contiene la declaraci�n del estado de juego.

@see Application::CGameState
@see Application::CMultiplayerTeamDeathmatchClientState

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Application_MultiplayerTeamDeathmatchClientState_H
#define __Application_MultiplayerTeamDeathmatchClientState_H

#include "GameState.h"
#include "Net/Manager.h"

#include <iostream>

namespace Application {
	/**
	@ingroup applicationGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/
	// Heredar de Net::IObserver para realizar una desconexion explicita
	// de lo contrario el timeout dispara el envio del paquete de desconexion
	class CMultiplayerTeamDeathmatchClientState : public CGameState, public Net::CManager::IObserver {
	public:
		/** Constructor de la clase */
		CMultiplayerTeamDeathmatchClientState(CBaseApplication *app) : CGameState(app) {}

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Funci�n llamada por la aplicaci�n cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/******************
			IOBSERVER
		******************/
		virtual void dataPacketReceived(Net::CPaquete* packet);
		virtual void connexionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }
		virtual void disconnexionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }
	}; // CMultiplayerTeamDeathmatchClientState

} // namespace Application

#endif //  __Application_GameState_H
