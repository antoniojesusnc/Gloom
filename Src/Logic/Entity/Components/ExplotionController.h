/**
@file ExplotionController.h

Contiene la declaraci�n de la cabecera del componente
que controla el timer de la explosion.

@see Logic::CExplotionController
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_ExplotionController_H
#define __Logic_ExplotionController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/
	
	class CExplotionController : public IComponent {
		DEC_FACTORY(CExplotionController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CExplotionController() : IComponent(), _timer(0), _enemyHit(false) { }


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		M�todo llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>explotionTime: </strong>tiempo en segundos tras el cual la granada explota.</li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTACT_ENTER</li>
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


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CEntity* _owner);

	private:


		// =======================================================================
		//                            METODOS PRIVADOS
		// =======================================================================

		/** Crea una entidad GrenadeExplotion justo en el lugar en el que se encuentre la granada (_entity). */
		void createExplotion();

		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Transcurrido este tiempo, la se destruye la entidad granada y se ejecuta la explosion. */
		float _explotionTime;

		/** Timer que controla cuando explota la granada. */
		unsigned int _timer;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

		/** true si la granada a golpeado a un enemigo */
		bool _enemyHit;

	}; // class CExplotionController

	REG_FACTORY(CExplotionController);

} // namespace Logic

#endif
