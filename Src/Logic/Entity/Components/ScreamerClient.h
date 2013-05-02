//---------------------------------------------------------------------------
// ScreamerClient.h
//---------------------------------------------------------------------------

/**
@file ScreamerClient.h

Contiene la declaraci�n de la clase que
implementa las habilidades del personaje
"Screamer" (Cliente).

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerClient_H
#define __Logic_ScreamerClient_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaraci�n de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer" (Cliente).

	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CScreamerClient : public CPlayerClass {
		DEC_FACTORY(CScreamerClient);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerClient();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamerClient();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
		

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>ACTIVATE_SCREAMER_SHIELD</li>
			<li>DEACTIVATE_SCREAMER_SHIELD</li>
			<li>SET_RELATED_ENTITY</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** 
		Absorbe todos los proyectiles que le llegan de frente. Esta habilidad tiene
		un l�mite, si se sobrepasa explotas.
		*/
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();


	protected:

		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);


	private:


		// =======================================================================
		//                          M�TODOS PRIVADOS
		// =======================================================================


		/** Activa el shader del escudo y a la entidad */
		void activateScreamerShield();

		//__________________________________________________________________

		/** Desactiva el shader del escudo y a la entidad. */
		void deactivateScreamerShield();

		//__________________________________________________________________

		/** Recoloca el escudo del screamer en la posici�n del punto de mira. */
		void refreshShieldGraphicsPos();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** true si el escudo est� activado */
		bool _shieldIsActive;

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

	}; // class CScreamerClient

	REG_FACTORY(CScreamerClient);

} // namespace Logic

#endif // __Logic_ScreamerClient_H
