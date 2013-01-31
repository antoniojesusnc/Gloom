//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementaci�n del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llans�
@date Agosto, 2010
*/

#include "GameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Physics/Server.h"

//#include <CEGUISystem.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>

namespace Application {

	bool CGameState::init() 
	{
		// Ejecutamos la inicializaci�n de la clase padre
		// En este caso no hace nada, solo retorna true
		CApplicationState::init();

		// INICIALIZACI�N DE LA F�SICA
		// ---------------------------

		// TODO: desactivar colisiones entre los grupos 0 y 1
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(0, 1, false);

		// TODO: Crear la escena f�sica usando el servidor de f�sica
		Physics::CServer::getSingletonPtr()->createScene();

		// INICIALIZACI�N DE LA L�GICA
		// ---------------------------

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		/*if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
			return false;*/

		// INICIALIZACI�N DEL GUI
		// ----------------------

		// Ahora mismo la implementaci�n est� totalmente acoplada a CEGUI
		// Hay que desacoplarlo utilizando un nuevo paquete donde se abstraiga
		// el subsistema utilizado

		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Time.layout");
		//_timeWindow = CEGUI::WindowManager::getSingleton().getWindow("Time");
		//LAYOUT TIME
		GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Time");
		
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Mira");

		return true;
	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{
		// Liberar los recursos reservados para la escena y las
		// clases construidas a partir de los blueprints
		Logic::CServer::getSingletonPtr()->unLoadLevel();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		// Liberar la escena f�sica usando el motor de f�sica
		Physics::CServer::getSingletonPtr()->destroyScene();

		// Llamar al m�todo padre por si acaso tiene que hacer algo
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameState::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

		// Activamos la ventana que nos muestra el tiempo transcurrido.
		//CEGUI::System::getSingletonPtr()->setGUISheet(_timeWindow);
		//_timeWindow->setVisible(true);
		//_timeWindow->activate();

		//LAYOUT TIME
		GUI::CServer::getSingletonPtr()->activateGUI(this, "Time");
	
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->activateGUI(this, "Mira");
	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		// Desactivamos la ventana de tiempo.
		//_timeWindow->deactivate();
		//_timeWindow->setVisible(false);

		//LAYOUT TIME
		GUI::CServer::getSingletonPtr()->deactivateGUI();

		// Desactivamos la clase que procesa eventos de entrada para 
		// controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida.
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		// TODO: realizar la simulaci�n f�sica
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la l�gica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;
		
		std::stringstream text;
		text << "Time: " << _time/1000;

		//TEXTO DEL LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->setText(text.str());

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->setState("menu");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
