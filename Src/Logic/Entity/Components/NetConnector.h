/**
@file NetConnector.h

Contiene la declaraci�n del componente que reenvia mensajes por la red.

@see Logic::CNet
@see Logic::IComponent

@author David Llans�
@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_NetConnector_H
#define __Logic_NetConnector_H

#include "Logic/Entity/Component.h"

#include <set>

// Declaraci�n de la clase
namespace Logic {

	/**
	Este componente es el nucleo de la comunicaci�n entre entidades
	a trav�s de la red.
	<p>
	Al dividir la entidad en el lado del servidor y del cliente, en 
	ambos lados la entidad tiene una instancia de esta clase. La
	misi�n de este componente es retransmitir mensajes mandados por
	los compoentes locales de su entidad al otro lado del tubo, 
	previa serializaci�n de esos mensajes.
	<p>
	Para elegir que mensajes se env�an por el tubo est� el atributo 
	"msgList" que es un string de ids de mensajes separado por comas.
	<p>
	En principio se envian todos los mensajes recibidos, sin embargo,
	para no saturar la red, se puede establecer un tiempo en el cual
	no se enviar� m�s de un mensaje del mismo tipo. Este atributo es
	"blockedTime" y se especifica en milisegundo. Esta aproximaci�n
	evita que se sature la red, pero no es una soluci�n viable para
	un juego. Otras t�cnicas deber�n ser aplicadas en los proyectos
	que usen red.

	@author David Llans�
	@date Diciembre, 2010
	*/
	class CNetConnector : public IComponent 
	{
		DEC_FACTORY(CNetConnector);
	public:

		/**
		Constructor por defecto.
		*/
		CNetConnector() : IComponent(), _timeOfBlocking(0) {}

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la c�mara gr�fica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		todos los tipos de mensajes especificados en el mapa.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		Se controla el n�mero de mensajes del mismo tipo que se env�an al 
		otro lado.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

	protected:

		/** 
		Arbol binario de busqueda que contiene los tipos de mensajes
		que este componente debe enviar por la red.
		*/
		std::set<Logic::TMessageType> _forwardedMsgTypes;

		/// Map que contiene los TMessageType de los mensajes
		/// que est�n bloqueados (no se env�an) debido a que se 
		/// acaba de enviar un mensaje del mismo tipo. El otro
		/// valor es el tiempo que falta para el desbloqueo.
		typedef std::map<Logic::TMessageType, int> TTimeToUnblockMsgDelivery;
		typedef std::pair<Logic::TMessageType, int> TTimeToUnblockMsgDeliveryPair;
		TTimeToUnblockMsgDelivery _timeToUnblockMsgDelivery;

		/// Milisegundos que se esperan entre envios de mensajes del mismo tipo
		unsigned int _timeOfBlocking;

	}; // class CNetConnector

	REG_FACTORY(CNetConnector);

} // namespace OIM

#endif // __Logic_NetConnector_H
