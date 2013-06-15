//---------------------------------------------------------------------------
// TDMServer.h
//---------------------------------------------------------------------------

/**
@file TDMServer.h

@see Application::CTDMServer
@see Application::CGameServerState

@author Francisco Aisa Garc�a
@date Junio, 2013
*/

#ifndef __Application_TDMServer_H
#define __Application_TDMServer_H

#include "GameServerState.h"

namespace Logic {
	class CEntity;
}

namespace Application {
	
	/**
	@ingroup applicationGroup

	@author Francisco Aisa Garc�a
	@date Junio, 2013
	*/

	class CTDMServer : public CGameServerState {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CTDMServer(CBaseApplication* app);

		void gameSettings(const std::vector<std::string>& mapList, bool loopMaps, const std::pair<unsigned int, unsigned int>& timeLimit, 
						  unsigned int goalScore, bool forceRespawn, bool warmUp, bool autoBalanceTeams);

		virtual void gameEventOcurred(Logic::CEntity* emitter, const std::shared_ptr<Logic::CMessage>& msg);


		// =======================================================================
		//                           METODOS HEREDADOS
		// =======================================================================

		virtual void tick(unsigned int msecs);

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		virtual void activate();

		//______________________________________________________________________________

		/**
		Funci�n llamada por la aplicaci�n cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		//______________________________________________________________________________

		// M�todos de CKeyboardListener

		/**
		M�todo que ser� invocado siempre que se pulse una tecla. 
		Ser� la aplicaci�n qui�n llame a este m�todo cuando el 
		estado est� activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyPressed(Input::TKey key);

		//______________________________________________________________________________

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla. Ser� la aplicaci�n qui�n llame a este m�todo 
		cuando el estado est� activo. Esta clase NO se registra en
		el InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyReleased(Input::TKey key);

		//______________________________________________________________________________

		// M�todos de CMouseListener

		/**
		M�todo que ser� invocado siempre que se mueva el rat�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mouseMoved(const Input::CMouseState &mouseState);

		//______________________________________________________________________________

		/**
		M�todo que ser� invocado siempre que se pulse un bot�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mousePressed(const Input::CMouseState &mouseState);

		//______________________________________________________________________________

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n. La aplicaci�n avisa de este evento al estado 
		actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners. 
		*/
		virtual bool mouseReleased(const Input::CMouseState &mouseState);

	private:

		void endGame();

		bool isPlayer(Logic::CEntity* entity);

		// Game Settings
		bool _forceRespawn;
		bool _warmUp;
		bool _autoBalanceTeams;

		// Map Control
		std::vector<std::string> _mapList; // Se respeta el orden que se le haya dado
		bool _loopMaps;
		unsigned int _currentMap;

		// Variables
		bool _inEndGame;
		unsigned int _blueTeamScore;
		unsigned int _redTeamScore;

	}; // CTDMServer

} // namespace Application

#endif // __Application_DMServer_H
