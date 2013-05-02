/**
@file CameraFeedbackNotifier.h

Contiene la declaraci�n del componente que 
controla los da�os recibidos por el escudo
del Screamer.

@see Logic::CCameraFeedbackNotifier
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Abril, 2013
*/

#ifndef __Logic_CameraFeedbackNotifier_H
#define __Logic_CameraFeedbackNotifier_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Abril, 2013
	*/
	
	class CCameraFeedbackNotifier : public IComponent {
		DEC_FACTORY(CCameraFeedbackNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CCameraFeedbackNotifier();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CCameraFeedbackNotifier();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>SET_REDUCED_DAMAGE</li>
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

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea el valor del porcentaje de da�os absorbido. Se entiende que
		se llama a este metodo cuando la entidad muere y vuelve a ser
		despertada.
		*/
		virtual void activate();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Manda un mensaje a la c�mara para que vibre
		*/
		void damaged();

		//__________________________________________________________________

		/**
		Este m�todo debe ser usado por aquella entidad que cree a esta entidad,
		para indicar que �sta �ltima le pertenece.

		@param owner Entidad l�gica que gener� a esta entidad.
		*/
		void setOwner(Logic::CEntity* owner);


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Entidad due�a de esta entidad. */
		CEntity* _owner;

	}; // class CScreamerShieldDamageNotifier

	REG_FACTORY(CCameraFeedbackNotifier);

} // namespace Logic

#endif // __Logic_CameraFeedbackNotifier_H