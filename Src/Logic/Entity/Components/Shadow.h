//---------------------------------------------------------------------------
// Shadow.h
//---------------------------------------------------------------------------

/**
@file Shadow.h

Contiene la declaraci�n de la clase que
implementa las habilidades del personaje
"Shadow".

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_Shadow_H
#define __Logic_Shadow_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**
	Clase que implementa las habilidades propias
	del personaje "Shadow".

	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CShadow : public CPlayerClass {
		DEC_FACTORY(CShadow);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShadow();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CShadow();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		De momento no lee nada nuevo, tan solo llama a la clase padre.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** El personaje se vuelve invisible al invocar este m�todo. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

	}; // class CShadow

	REG_FACTORY(CShadow);

} // namespace Logic

#endif // __Logic_Shadow_H
