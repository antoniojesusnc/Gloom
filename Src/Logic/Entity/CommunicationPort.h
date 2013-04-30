/**
@file CommunicationPort.h

Contiene la declaraci�n de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llans� Garc�a
@author Francisco Aisa Garc�a
@date Abril, 2013
*/

#ifndef __Logic_CommunicationPort_H
#define __Logic_CommunicationPort_H

#include <list>
#include <memory>

#include "Logic/Messages/Message.h"

// Declaraci�n de la clase
namespace Logic {

	/**
	La clase CCommunicationPort se encarga de toda la gestion de 
	los mensajes que pueden ser enviados. la interfaz de componente
	heredar� de esta clase para delegar en ella toda la comunicaci�n
	entre componentes. 

	En cualquier momento se puede enviar un mensaje mediante el 
	m�todo set() que ver� si el mensaje es aceptado y en ese caso
	lo meter� en una cola para que sea procesado posteriormente. Los 
	mensajes por tanto no se procesan autom�ticamente sino que se 
	guardan y posteriormente, generalmente una vez por frame, se
	procesan todos los mensajes recibidos invocando al m�todo
	processMessages(), siempre y cuando el estado del componente lo
	permita.

	Para que esto funcione correctamente, las clases hijas deber�n
	implementar los m�todos virtuales accept() y process().
	El m�todo accept() decidir� si el mensaje recibido es aceptado 
	o rechazado, mientras que el m�todo process() se encargar� de 
	procesar un mensaje concreto.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans� Garc�a
	@author Francisco Aisa Garc�a
	@date Abril, 2013
	*/

	class CCommunicationPort {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CCommunicationPort();

		//__________________________________________________________________

		/** Destructor. Vac�a el vector de mensajes. */
		virtual ~CCommunicationPort();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		M�todo que a�ade un mensaje a la cola si �ste es aceptado.

		@param message Mensaje a procesar.
		@return true si el mensaje ha sido admitido y puesto en cola.
		*/
		bool set(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que elige que mensajes son aceptados. Las clases
		que hereden del puerto de comunicaci�n deber�n reimplementar
		este m�todo si quieren aceptar alg�n mensaje ya que por defecto
		se rechazan todos.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo que procesa la lista de mensajes que faltan por procesar.
		Simplemente invoca a process() con cada uno de los mensajes.
		
		@return true Si se ha procesado algun mensaje.
		*/
		bool processMessages();

		//__________________________________________________________________

		/** Limpia la lista de mensajes a procesar. */
		void clearMessages();


	protected:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** typedef para la lista de mensajes. */
		typedef std::list< std::shared_ptr<CMessage> > CMessageList;

		//__________________________________________________________________

		/**Lista de mensajes por procesar. */
		CMessageList _messages;

	}; // CCommunicationPort

} // namespace Logic

#endif // __Logic_CommunicationPort_H
