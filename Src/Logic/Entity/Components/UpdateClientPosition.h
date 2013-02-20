/**
@file UpdateClientPosition.h

Contiene la declaraci�n del componente que envia con cierta frecuencia
desde el servidor al cliente la posicion que existe en el servidor.

@author Francisco Aisa Garc�a
@date Octubre, 2010
*/

#ifndef __Logic_UpdateClientPosition_H
#define __Logic_UpdateClientPosition_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CUpdateClientPosition : public IComponent {
		DEC_FACTORY(CUpdateClientPosition);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. Inicializa el timer a 0. */
		CUpdateClientPosition() : IComponent(), _timer(0) {}


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
			<li><strong>syncPosTimeStamp:</strong> Tiempo de recolocacion del player </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		/** Timer para controlar cada cuanto se manda el mensaje de sincronizacion de la posicion del cliente. */
		float _timer;

		/** Limite de tiempo para mandar el mensaje de sincronizacion. */
		float _syncPosTimeStamp;
	}; // class CUpdateClientPosition

	REG_FACTORY(CUpdateClientPosition);

} // namespace Logic

#endif
