/**
@file SpectatorController.h

Contiene la declaraci�n del componente que controla el movimiento 
del espectador.

@see Logic::CSpectatorController
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Abril, 2013
*/

#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CMessageControl;
	class CPhysicController;
}

//declaraci�n de la clase
namespace Logic {

	/**
	
	*/

	class CSpectatorController : public IComponent {
		DEC_FACTORY(CSpectatorController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CSpectatorController();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual void activate();

		virtual void onStart(unsigned int msecs);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		

		/**
		Provoca que la entidad gire. N�meros Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/

		/**
		Dado un giro en X y otro en Y provoca que la entidad (y la camara
		giren).

		@param XYturn array con los valores de giro del raton.
		*/
		void mouse(float XYturn[]);

		
	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================


		Vector3 estimateMotionDirection();

		void estimateMotion(unsigned int msecs);

		void executeMovementCommand(ControlType commandType);

		void normalizeDirection();

		void initMovementCommands();


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		/** Velocidad m�xima a la que nuestro personaje se puede desplazar. */
		float _maxVelocity;

		/** 
		Coeficiente de rozamiento del aire, entre 0 y 1. A menor valor, menor
		recorrido en el aire. 
		*/
		float _frictionCoef;

		/** Velocidad de aceleraci�n del personaje al desplazarse. */
		float _acceleration;

		/** Vector que indica la direcci�n del desplazamiento que el controlador debe realizar. */
		Vector3 _displacementDir;

		/** Vector de inercia. */
		Vector3 _momentum;

		/** 
		Array que contiene los vectores que corresponden a cada uno de los movimientos
		de desplazamiento y salto que se pueden realizar. 
		*/
		Vector3 _movementCommands[8];




		/** Puntero al controlador fisico del player. */
		CPhysicController* _physicController;

	}; // class CSpectatorController

	REG_FACTORY(CSpectatorController);

} // namespace Logic

#endif // __Logic_AvatarController_H
