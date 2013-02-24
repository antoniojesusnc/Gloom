/**
@file ExplotionHitNotifier.h

Contiene la declaraci�n de la cabecera del componente
que se encarga de mandar el mensaje de da�o a aquellas
entidades que se vean afectadas por la explosion.

@see Logic::CExplotionHitNotifier
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_ExplotionHitNotifier_H
#define __Logic_ExplotionHitNotifier_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/
	
	class CExplotionHitNotifier : public IComponent {
		DEC_FACTORY(CExplotionHitNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CExplotionHitNotifier() : IComponent() { }


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
			<li><strong>explotionDamage: </strong>Da�o m�ximo que puede llegar a hacer la explosi�n.</li>
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
			<li>TOUCHED</li>
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

		//________________________________________________________________________

		/**
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CEntity* _owner);

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		void explotionHit(CEntity* entity);


		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Da�o m�ximo que puede realizar la explosi�n. */
		float _explotionDamage;

		/** Radio de acci�n de la onda expansiva. */
		float _explotionRadius;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

	}; // class CExplotionHitNotifier

	REG_FACTORY(CExplotionHitNotifier);

} // namespace Logic

#endif
