//---------------------------------------------------------------------------
// GameClientState.h
//---------------------------------------------------------------------------

/**
@file GameClientState.h

Contiene la declaraci�n del estado de juego.

@see Application::CGameState
@see Application::CGameClientState

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Application_GameClientState_H
#define __Application_GameClientState_H

#include "GameState.h"
#include "Net/Manager.h"
#include "FlashValue.h"
#include <iostream>

namespace Hikari{
	class FlashControl;
}

namespace Net {
	class CManager;
}

namespace Application {
	
	/**
	@ingroup applicationGroup

	Representa el estado de juego online para el cliente. En el futuro
	tendremos diferentes estados por cada modo de juego, pero de momento
	se queda asi hasta que por lo menos tengamos un deathmatch.

	Hereda de CGameState por que es un estado de juego y de Net::CManager::IObserver
	para que se le avise de que ha recibido paquetes (en este caso los
	paquetes de datos son los unicos que el cliente va a querer).

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	@deprecated Actualmente hereda de IObserver pero IObserver tiene listeners
	que no interesan al cliente (como las llamadas de conexion y desconexion).
	*/

	class CGameClientState : public CGameState, public Net::CManager::IObserver {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CGameClientState(CBaseApplication *app) : CGameState(app) {}


		// =======================================================================
		//                           METODOS HEREDADOS
		// =======================================================================

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/

		virtual bool init();

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
		
		/**
		Se dispara cuando se recibe un paquete de datos.

		@see Net::CManager::IObserver
		@param packet Paquete de datos recibido.
		*/
		virtual void dataPacketReceived(Net::CPaquete* packet);

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de conexion. En el caso del cliente
		este metodo no deberia dispararse nunca.

		@see Net::CManager::IObserver
		@param packet Paquete de conexion recibido.
		*/
		virtual void connectionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de desconexion.

		@see Net::CManager::IObserver
		@param packet Paquete de desconexion recibido.
		*/
		virtual void disconnectionPacketReceived(Net::CPaquete* packet);

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

		// M�todos de CMouseListener
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mouseMoved(const Input::CMouseState &mouseState);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mousePressed(const Input::CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n. La aplicaci�n avisa de este evento al estado 
		actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners. 
		*/
		virtual bool mouseReleased(const Input::CMouseState &mouseState);

		Hikari::FlashValue classSelected(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		void basicGameSettings(unsigned int timeLimit, unsigned int goalScore);

	protected:

		void disconnect();

		unsigned int _npings;
		unsigned int _pingActual;

		//menu de seleccion de personaje
		Hikari::FlashControl* _seleccion;

		Net::CManager* _netMgr;

		//�apa porque flash hace lo que le sale de los huevos

		private:

		bool _menuVisile;
	}; // CMultiplayerTeamDeathmatchClientState

} // namespace Application

#endif //  __Application_GameClientState_H
