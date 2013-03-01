/**
@file Interpolation.h

Contiene la declaraci�n del componente que realiza la interpolacion
del movimiento cuando el servidor nos informa de la posicion que deberiamos
tener.

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_Interpolation_H
#define __Logic_Interpolation_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	/**
    @ingroup logicGroup

	Clase encargada de recolocar al jugador en caso de no estar sincronizados
	con el servidor.

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CInterpolation : public IComponent {
		DEC_FACTORY(CInterpolation);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto */
		CInterpolation() : IComponent() {}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa. Actualmente este componente no lee nada para inicializarse.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Tick de reloj del componente
		*/
		virtual void tick();


		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>SYNC_POSITION</li>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		//________________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		/**
		M�todo que mueve el punto del servidor para que est� actualizado
		*/
		void moveServerPos();
		/**
		calcula si hay que hacer interpolaci�n, o mueve al player si la distancia es
		muy grande
		*/
		void calculateInterpolation();

	private:
		/**
		Posici�n que el servidor me ha dicho que es donde debo estar
		*/
		Matrix4 _serverPos;
		float _yawDifference;
		/**
		distancia maxima a la que interpolo poco a poco
		*/
		float _maxDistance;
		/**
		distancia minima a la que interpolo poco a poco
		*/
		float _minDistance;
		/**
		minima diferencia de yaw que puede haber entre servidor y cliente
		*/
		float _minYaw;
		/**
		maxima diferencia de yaw que puede haber entre servidor y cliente
		*/
		float _maxYaw;
		/**
		minima diferencia de pitch que puede haber entre servidor y cliente
		*/
		float _minPitch;
		/**
		maxima diferencia de pitch que puede haber entre servidor y cliente
		*/
		float _maxPitch;
		/**
		Variables de control de interpolaci�n (para no liarla)
		*/
		bool _interpolating;
		bool _canInterpolateMove;
		bool _canInterpolateRotation;
		/**
		variable que indica el ping con el que estamos corrigiendo
		*/
		unsigned int _actualPing;
		/**
		direcci�n en la que se est� moviendo el server
		*/
		Vector3 _serverDirection;

		float _speed;
		float _rotationSpeed;
	}; // class CInterpolation

	REG_FACTORY(CInterpolation);

} // namespace Logic

#endif // __Logic_Life_H
