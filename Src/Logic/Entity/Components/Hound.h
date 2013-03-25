//---------------------------------------------------------------------------
// Hound.h
//---------------------------------------------------------------------------

/**
@file Hound.h

@author 
@date Marzo, 2013
*/

#ifndef __Logic_Hound_H
#define __Logic_Hound_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**


	@ingroup logicGroup

	@author 
	@date Febrero, 2013
	*/

	class CHound : public CPlayerClass {
		DEC_FACTORY(CHound);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CHound() {
			// No hay memoria dinamica que inicializar
		}

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CHound() { }
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                     METODOS HEREDADOS DE CSkills
		// =======================================================================


		/**
		*/
		virtual void primarySkill() { /* Implementar */ };

		//__________________________________________________________________

		/**
		*/
		virtual void secondarySkill() { /* Implementar */ };


	protected:


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


	}; // class CHound

	REG_FACTORY(CHound);

} // namespace Logic

#endif // __Logic_Hound_H
