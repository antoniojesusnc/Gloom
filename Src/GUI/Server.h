/**
@file Server.h

Contiene la declaraci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la interfaz con el usuario (entrada de perif�ricos, CEGui...).

@see GUI::CServer

@author David Llans�
@date Agosto, 2010
*/
#ifndef __GUI_Server_H
#define __GUI_Server_H

#include <string>
#include <map>
#include <set>
#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CAvatarController;
}

namespace CEGUI
{
	class System;
	class Window;
}

namespace Hikari {
	class HikariManager;
	class FlashControl;
	struct Position;
}

namespace Application {
	class CApplicationState;
}

namespace GUI {
	class GUIDescriptor;
	class GUIEventArgs;
}

typedef std::map<Application::CApplicationState*, std::map<std::string, GUI::GUIDescriptor*> > StateLayoutTable;

// Declaraci�n de la clase
namespace GUI
{
	/**
	Servidor del m�dulo GUI que hace de interfaz con el usuario. Est� 
	implementado como un singlenton de inicializaci�n expl�cita. Sirve 
	para comunicar a CEGUI los eventos producidos ya as� hacer funcionar
	la interfaz gr�fica de usuario y permite que se establezca que entidad 
	es controlada por el GUI::CPlayerController. Este control se hace a 
	trav�s del componente Logic::CAvatarController.

	@ingroup GUIGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CServer
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/***************************************************************
		M�todos para la configuraci�n del GUI
		***************************************************************/

		/**
		 * Create a new GUI descriptor for a new layout.
		 *
		 * In the previous version, game states had just one possible 
		 * GUI layout. The point here is to allow to swap between
		 * different layouts in the same game state.
		 *
		 * If the state doesn't exist or the layout name doesn't exist, 
		 * it creates a new slot in the table.
		 * 
		 * CAREFUL! If they do exist (state and layout), they will be 
		 * overwrited and their pointers will be lost, causing to leak 
		 * memory. Hence, WE SHOULD NOT CREATE LAYOUTS WITH THE SAME
		 * NAME!!!
		 *
		 * @param state Game state that is going to have a new layout.
		 * @param layoutName Name of the new layout.
		 */
		void addLayoutToState(Application::CApplicationState* state, const std::string& layoutName);

		/**
		M�todo de creacion de un layout con un nombre, posicion y medidas del layout. Devuelve
		un controlador de flash con la informacion necesaria para utilizar la GUI

		@param layoutName Nombre del layout
		@param pos posici�n del layout
		@param width ancho del layout (en pixels)
		@param height alto del layout (en pixels)

		@return un FlashControl con el controlador del layout, listo para cargar el swf y bindear y
		ejecutar todas las llamadas que quieras la interfaz de flash
		*/

		Hikari::FlashControl* addLayout(const std::string& layoutName, Hikari::Position pos, unsigned int width, unsigned int height);

		/**
		M�todo de creacion de un layout con un nombre, posicion y medidas del layout. Devuelve
		un controlador de flash con la informacion necesaria para utilizar la GUI

		@param layoutName Nombre del layout
		@param pos posici�n del layout
		@param relativePos posici�n relativa con respecto al tama�o total de la pantalla

		@return un FlashControl con el controlador del layout, listo para cargar el swf y bindear y
		ejecutar todas las llamadas que quieras la interfaz de flash
		*/
		Hikari::FlashControl* addLayout(const std::string& layoutName, Hikari::Position pos, float relativePos);

		/**
		M�todo de creacion de un layout con un nombre, posicion y con el tama�o total de la pantalla. Devuelve
		un controlador de flash con la informacion necesaria para utilizar la GUI

		@param layoutName Nombre del layout
		@param pos posici�n del layout

		@return un FlashControl con el controlador del layout, listo para cargar el swf y bindear y
		ejecutar todas las llamadas que quieras la interfaz de flash
		*/
		Hikari::FlashControl* addLayout(const std::string& layoutName, Hikari::Position pos);

		/**
		M�todo de creacion de un layout con un nombre, posicion y con el tama�o total de la
		pantalla. Devuelve un controlador de flash con la informacion necesaria para utilizar 
		la GUI.

		@param state estado en el que se est� creando el layout
		@param layoutName Nombre del layout
		@param pos posici�n del layout

		@return un FlashControl con el controlador del layout, listo para cargar el swf y bindear y
		ejecutar todas las llamadas que quieras la interfaz de flash
		*/
		Hikari::FlashControl* addLayoutToState(Application::CApplicationState* state, const std::string& layoutName, Hikari::Position pos);

		//________________________________________________________________________

		/**
		 * Add a new button to an existing layout of an existing game state.
		 *
		 * @param state Game state that holds the layout to which we want to add
		 * a button to.
		 * @param layoutName Name of the layout that is going to have a new button
		 * @param buttonName Name of the new button.
		 * @param buttonFunction Function that is going to be fired whenever the
		 * button that we are adding is clicked.
		 */
		void addButtonToLayout( Application::CApplicationState* state, const std::string& layoutName, 
			const std::string& buttonName, bool (*buttonFunction)(const GUIEventArgs&) );

		//________________________________________________________________________

		/**
		 * Makes the window associated with the given layout active (meaning it 
		 * shows that layout on screen).
		 *
		 * @param state Game state to which the layout that wants to be showed 
		 * belongs to.
		 * @param layoutName Name of the layout that is going to be activated.
		 */
		bool activateGUI(Application::CApplicationState* state, const std::string& layoutName);
		
		//________________________________________________________________________

		/** Show the mouse cursor. */
		void activateMouseCursor();

		//________________________________________________________________________

		/** Deactivates the active GUI */
		void deactivateGUI();
		
		//________________________________________________________________________

		/** Hides the mouse cursor */
		void deactivateMouseCursor();

		//________________________________________________________________________

		/**
		 * Show a text on the GUI screen.
		 *
		 * @param msg Message that is going to be shown on screen. 
		 */
		void setText(const std::string& msg);

		void tick();

	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		/**
		 * Puntero a la ventana CEGUI que esta actualmente siendo renderizada.
		 * Si no se esta renderizando ninguna interfaz de usuario es NULL
		 */
		CEGUI::Window* _currentWindow;

		/** 
		 * Table to hold pointers to state and their respective possible layouts
		 * Once a layout is selected, it is loaded using the name of the layout 
		 * and the configuration set when the state was fired.
		 * Note that this implies that layout names should match .layout file 
		 * names; the same goes for button names.
		 */
		StateLayoutTable _layoutTable;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;

		Hikari::HikariManager* _manager;

	}; // class CServer

} // namespace GUI

#endif // __GUI_Server_H
