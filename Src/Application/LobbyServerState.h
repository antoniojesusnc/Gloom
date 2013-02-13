//---------------------------------------------------------------------------
// LobbyServerState.h
//---------------------------------------------------------------------------

/**
@file LobbyServerState.h

Contiene la declaraci�n del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_LobbyServerState_H
#define __Application_LobbyServerState_H

#include "ApplicationState.h"
#include "Net/Manager.h"
#include <list>
#include <map>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del men�
	principal del juego. Es muy sencilla y lo �nico que hace es cargar
	un layout de CEGUI al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haci�ndo visible/invisible tambi�n
	el puntero del rat�n). Tambi�n asocia los eventos de los botones 
	del men� a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es CEGUI dependiente, lo cual no es deseable, la aplicaci�n
	deber�a ser independiente de las tecnolog�as usadas.

	@ingroup applicationGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CLobbyServerState : public CApplicationState , public Net::CManager::IObserver
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CLobbyServerState(CBaseApplication *app) : CApplicationState(app), 
											       _waiting(true), 
												   _playersFetched(0) 
		{
			// Nada que hacer
		}

		/** 
		Destructor 
		*/
		virtual ~CLobbyServerState();

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

		/**
		Funci�n llamada por la aplicaci�n para que se ejecute
		la funcionalidad del estado.

		@param msecs N�mero de milisegundos transcurridos desde
		la �ltima llamada (o desde la �ctivaci�n del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

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
		virtual bool keyPressed(GUI::TKey key);
		
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
		virtual bool keyReleased(GUI::TKey key);

		// M�todos de CMouseListener
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n. La aplicaci�n avisa de este evento al estado 
		actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState);


		/******************
			IOBSERVER
		******************/
		virtual void dataPacketReceived(Net::CPaquete* packet);
		virtual void connexionPacketReceived(Net::CPaquete* packet);
		virtual void disconnexionPacketReceived(Net::CPaquete* packet);


	private:

		/**
		Ventana CEGUI que muestra el men�.
		*/
		CEGUI::Window* _menuWindow;

		typedef std::list<Net::NetID> TNetIDList;

		/**
		lista donde almacenamos los clientes conectados
		*/
		TNetIDList _clients;

		/**
		lista donde almacenamos los clientes que han cargado el mapa
		*/
		TNetIDList _mapLoadedByClients;

		typedef std::map<Net::NetID,unsigned int> TNetIDCounterMap;
		typedef std::pair<Net::NetID,unsigned int> TNetIDCounterPair;

		/**
		Tabla donde almacenamos por cada cliente cuantos jugadores han creado
		*/
		TNetIDCounterMap _playersLoadedByClients;
		
		/** Contador para la informacion obtenida de los players */
		unsigned int _playersFetched;

		/**
		Indica si estamos en fase de espera de jugadores
		*/
		bool _waiting;

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n start.
		Simplemente cambia al estado de juego.
		*/
		bool startReleased(const CEGUI::EventArgs& e);

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n back.
		Simplemente cambia al estado de menu.
		*/
		bool backReleased(const CEGUI::EventArgs& e);

		/**
		* Funci�n que ejecuta la acci�n start. 
		Centraliza el c�digo y ser� invocada cuando se pulse la tecla correspondiente o se
		genere el evento de rat�n
		*/
		void doStart();

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
