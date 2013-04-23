/**
@file RocketControllerClient.h

Contiene la declaraci�n de la cabecera del componente
que controla la explosi�n del cohete sin quitar vida.

@see Logic::CRocketControllerClient
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Marzo, 2013
*/

#ifndef __Logic_RocketControllerClient_H
#define __Logic_RocketControllerClient_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez
	@date Abril, 2013
	*/
	
	class CRocketControllerClient : public IComponent {
		DEC_FACTORY(CRocketControllerClient);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CRocketControllerClient();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


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
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


	private:


		// =======================================================================
		//                            METODOS PRIVADOS
		// =======================================================================

		/** Crea una entidad GrenadeExplotion justo en el lugar en el que se encuentre la granada (_entity). */
		void createExplotion();


		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Booleano que controla que solo se trate el primer contacto ( debe explotar ). */
		bool _explotionActive;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

		/** Ruta del sonido de la explosion. */
		std::string _audioExplotion;

	}; // class CRocketControllerClient

	REG_FACTORY(CRocketControllerClient);

} // namespace Logic

#endif
