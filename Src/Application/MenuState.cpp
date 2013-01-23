//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementaci�n del estado de men�.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llans�
@date Agosto, 2010
*/

#include "MenuState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el men�
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Menu.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Menu");
		
		// Asociamos los botones del men� con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("Menu/Multiplayer")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::multiplayerReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el men� y el rat�n.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{

		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;

		case GUI::Key::RETURN:
			_app->setState("game");
			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
				return false;
			// Cargamos el nivel a partir del nombre del mapa. 
			if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
				return false;
			
			// TODO Llamar al m�todo de creaci�n del jugador. Deberemos decidir
			// si el jugador es el jugador local. Al ser el monojugador lo es.
			Logic::CServer::getSingletonPtr()->getMap()->createPlayer("Mono", true);

			// HACK Deber�amos poder propocionar caracter�asticas
			// diferentes seg�n el cliente (nombre, modelo, etc.). Esto es una
			// aproximaci�n, solo cambiamos el nombre y decimos si es el jugador
			// local
			break;

		case GUI::Key::M:
			_app->setState("netmenu");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
// TODO Por qu� se devuelve  true o false?
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("game");

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
			return false;

		// TODO Llamar al m�todo de creaci�n del jugador. Deberemos decidir
		// si el jugador es el jugador local. Al ser el monojugador lo es.
		Logic::CServer::getSingletonPtr()->getMap()->createPlayer("Mono", true);

		// HACK Deber�amos poder propocionar caracter�asticas
		// diferentes seg�n el cliente (nombre, modelo, etc.). Esto es una
		// aproximaci�n, solo cambiamos el nombre y decimos si es el jugador
		// local

		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CMenuState::multiplayerReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("netmenu");
		return true;

	} // multiplayerReleased

} // namespace Application
