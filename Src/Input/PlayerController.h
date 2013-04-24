/**
@file PlayerController.h

Contiene la declaraci�n de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del rat�n y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Input_PlayerController_H
#define __Input_PlayerController_H

#include "InputManager.h"



// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
}

// Declaraci�n de la clase
namespace Input
{
	/**
	Esta clase sirve para gestionar el teclado y el rat�n, y mover un avatar 
	acorde con las pulsaciones y el movimiento. En el constructor se registra en 
	el gestor de teclado y en el de rat�n (InputManager) para ser avisado ante 
	los eventos. No obstante, puede activarse y desactivarse a voluntad con 
	activate() y deactivate(). El componente CAvatarController del jugador
	es responsable de ponerse como foco a traves de GUI::CServer y as� recibir 
	las instrucciones dadas por esta clase.

	@ingroup GUIGroup

	@author David Llans�
	@date Agosto, 2010
	*/

	//'Constante' de m�ximo tiempo para dar la doble pulsaci�n del salto hacia los lados
	#define MAX_TIME_DOBULE_PUSH 150

	class CPlayerController : public CKeyboardListener, public CMouseListener
	{
	public:
		/**
		Constructor.
		*/
		CPlayerController ();

		/**
		Destructor.
		*/
		~CPlayerController();

		/**
		Establece el componente del jugador al que enviaremos acciones 
		de movimiento en funci�n de las teclas pulsadas y el movimiento 
		del rat�n.

		@param avatarController Componente al que enviaremos acciones de 
		movimiento en funci�n de las teclas pulsadas y el movimiento del 
		rat�n.
		*/
		void setControlledAvatar(Logic::CEntity *controlledAvatar) 
										{_controlledAvatar = controlledAvatar;} 

		Logic::CEntity* getControllerAvatar(){
			return _controlledAvatar;
		}
		/**
		Activa la la clase, se registra en el CInputManager y as� empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/

		/**
		M�todo que ser� invocado siempre que se pulse una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyPressed(TKey key);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyReleased(TKey key);

		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/

		/**
		M�todo que ser� invocado siempre que se mueva el rat�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se pulse un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState);

		/**
		Enumerado con las acciones de movimiento
		*/
		enum E_MOVE {LEFT, RIGHT, WALK, WALKBACK };

	protected:

		/**
		Entidad jugador al que enviaremos acciones de movimiento en
		funci�n de las teclas pulsadas y el movimiento del rat�n.
		*/
		Logic::CEntity *_controlledAvatar;


	private:
		//-------------------------------------------
		//             M�todos privados
		//------------------------------------------

		/**
		M�todo para saber de qu� tipo es la tecla pulsada. Para una lectura de c�digo m�s clara
		@param key Tecla pulsada.
		@return int con el tipo de tecla 0 = Movimiento; 1 = Cambio de arma; 2 = Habilidad ; 
				3=Otro (Ej.Debug) ; -1 = Ninguna tecla asignada
		*/
		int typeOfKey(TKey key);

		/**
		M�todo para emitir el mensaje de cambiar de arma, en funci�n de la tecla de arma que se pulse
		@param key Tecla pulsada.
		*/
		void ChangeWeaponMessage(TKey key);

		/**
		M�todo para emitir el mensaje de movimiento
		@param key Tecla pulsada.
		*/
		void MovementMessage(TKey key);

		/**
		M�todo para emitir el mensaje de la habilidad
		@param key Tecla pulsada.
		*/
		void HabilityMessage(TKey key);

		/**
		M�todo para emitir otros mensajes como los de debug, quitarse vida, o tecla escape
		@param key Tecla pulsada.
		*/
		void OtherMessages(TKey key);


		void ScrollWheelChangeWeapon(const CMouseState &mouseState);

		void ChangeWeaponMessage(int iWeapon);

		//-------------------------------------------
		//             Variables privados
		//------------------------------------------

		/**
		Tiempo de la �ltima pulsaci�n
		*/
		unsigned int m_iLastTime;

		/**
		�ltimo movimiento realizado
		*/
		E_MOVE m_eLastMove;

		int m_iLastScrollWheelPos;

		int m_iCurrentWeapon;

	}; // class CPlayerController

}

#endif // __GUI_PlayerController_H
