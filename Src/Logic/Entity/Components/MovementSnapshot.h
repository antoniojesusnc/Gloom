/**
@file Interpolation.h

Contiene la declaraci�n del componente que realiza la interpolacion
del movimiento cuando el servidor nos informa de la posicion que deberiamos
tener.

@author Francisco Aisa Garc�a
@author Ruben Mulero Guerrero
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

	class CMovementSnapshot : public IComponent {
		DEC_FACTORY(CMovementSnapshot);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto */
		CMovementSnapshot() : IComponent() {}


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

	protected:

		/** Tick de reloj del componente. */
		virtual void onFixedTick(unsigned int msecs);
	}; // class CMovementSnapshot

	REG_FACTORY(CMovementSnapshot);

} // namespace Logic

#endif // __Logic_Life_H
