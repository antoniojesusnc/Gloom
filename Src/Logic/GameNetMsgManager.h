//---------------------------------------------------------------------------
// GameNetMsgManager.h
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.h

Contiene la declaraci�n del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llans�
@date Febrero, 2011
*/

#ifndef __Net_gameNetMsgManager_H
#define __Net_gameNetMsgManager_H

#include "Net/Manager.h"

#include "Logic/Messages/Message.h"
#include "Logic/Maps/EntityID.h"

namespace Logic 
{
	/**
	Este m�dulo es un singleton que se usa como front-end de la red en el 
	estado de juego. Su mayor uso es desde la l�gica, aunque tambi�n 
	gestiona otros eventos.
	<p>
	La clase hace de oyente de los mensajes que se env�an por red durante
	el estado de juego. Su funci�n es decidir que hacer con cada tipo de 
	mensaje recibido. Principalmente recibe mensajes relacionados con la 
	l�gica de las entidades as� que los reenv�a a las entidades a las que
	corresponde No obstante tambi�n puede recibir otros tipos de mensajes 
	de control. 
	<p>
	Del mismo modo que se encarga de la deserializaci�n y reenv�o de los 
	mensajes l�gicos que vienen encapsulados por la red, proporciona un
	m�todo sendEntityMessage que se encarga de serializar y enviar al otro
	extremo del tubo un mensaje l�gico.

	@ingroup netGroup

	@author David Llans�
	@date Febrero, 2011
	*/
	// TODO Implementar Net::CManager::IObserver para poder recibir 
	// paquetes de datos (mensajes de red)
	class CGameNetMsgManager : public Net::CManager::IObserver
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CGameNetMsgManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CGameNetMsgManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada para activar la escucha.
		*/
		void activate();

		/**
		Funci�n llamada desactivar la escucha.
		*/
		void deactivate();

		/**
		M�todo que serializa un mensaje l�gico y lo env�a por el tubo al otro
		extremo de la red.
		
		@param message Mensaje que debe ser serializado y enviado.
		@param destination ID de la entidad a la que va dirigido el mensaje.
		*/
		void sendEntityMessage(CMessage* message, TEntityID destination);

		/// IOBSERVER
		virtual void dataPacketReceived(Net::CPaquete* packet);
		virtual void connexionPacketReceived(Net::CPaquete* packet){}
		virtual void disconnexionPacketReceived(Net::CPaquete* packet){}

	protected:
		
		/** 
		Constructor de la clase 
		*/
		CGameNetMsgManager();

		/** 
		Destructor 
		*/
		~CGameNetMsgManager();

		/**
		M�todo que recibe un buffer que encapsula un mensaje de la l�gica que 
		debe ser transmitido a una entidad concreta. El m�todo se encarga de
		deserializar el mensaje, recuperar la entidad concreta y enviarle el
		mensaje l�gico a �sta.

		@param packet Datos con la serializaci�n del mensaje.
		*/
		void processEntityMessage(Net::CPaquete* packet);

	private:
		/**
		�nica instancia de la clase.
		*/
		static CGameNetMsgManager* _instance;

	}; // CGameNetMsgManager

} // namespace Logic

#endif //  __Net_gameNetMsgManager_H
