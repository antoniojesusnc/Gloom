//---------------------------------------------------------------------------
// LobbyClientState.h
//---------------------------------------------------------------------------

/**
@file LobbyClientState.h

Contiene la declaraci�n del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_LobbyClientState_H
#define __Application_LobbyClientState_H

#include "ApplicationState.h"
#include "Net/Manager.h"
#include "FlashValue.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application {
	class CBaseApplication;
}

namespace Hikari {
	class FlashControl;
}

namespace Application {

	/**
	Como su nombre indica, esta clase es la clase del men�
	principal del juego. Es muy sencilla y lo �nico que hace es cargar
	un layout de hikari al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haci�ndo visible/invisible tambi�n
	el puntero del rat�n). Tambi�n asocia los eventos de los botones 
	del men� a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es hikari dependiente, lo cual no es deseable, la aplicaci�n
	deber�a ser independiente de las tecnolog�as usadas.

	@ingroup applicationGroup

	@author David Llans�
	@date Agosto, 2010
	*/

	class CLobbyClientState : public CApplicationState, 
							  public Net::CManager::IObserver {
	public:
		
		/** 
		Constructor de la clase 
		*/
		CLobbyClientState(CBaseApplication *app);

		/** 
		Destructor 
		*/
		virtual ~CLobbyClientState();

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Funci�n llamada cuando se elimina ("desengancha") el
		estado de la aplicaci�n.
		*/
		virtual void release();

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

		virtual void tick(unsigned int msecs);


		virtual void dataPacketReceived(Net::CPaquete* packet);

		virtual void connectionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }

		virtual void disconnectionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }


		bool loadMapThread();

	private:

		/**
		layout de hikari que muestra el menu
		*/
		Hikari::FlashControl* _menu, *_loadMenu;

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n start.
		Simplemente cambia al estado de juego.
		*/
		Hikari::FlashValue startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n back.
		Simplemente cambia al estado de menu.
		*/
		Hikari::FlashValue backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);


		/**
		* Funci�n que ejecuta la acci�n start. 
		Centraliza el c�digo y ser� invocada cuando se pulse la tecla correspondiente o se
		genere el evento de rat�n
		*/
		void requestConnectionTo(const std::string& ip);

		void sendPlayerInfo();

		bool loadMap(const std::string& mapName);

		Net::CManager* _netMgr;

		std::string _mapName;

		std::shared_ptr<boost::thread> loadThread;

		GameMode::Enum _gameMode;
		int _gameTimeLeft;
		unsigned int _goalScore;

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H