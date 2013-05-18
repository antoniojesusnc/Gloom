/**
@file ScreamerShieldDamageNotifier.h

Contiene la declaraci�n del componente que 
controla los da�os recibidos por el escudo
del Screamer.

@see Logic::CScreamerShieldDamageNotifier
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerShieldDamageNotifier_H
#define __Logic_ScreamerShieldDamageNotifier_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CPhysicDynamicEntity;
}

namespace Logic {
	
	/**
	
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	
	class CScreamerShieldDamageNotifier : public IComponent {
		DEC_FACTORY(CScreamerShieldDamageNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerShieldDamageNotifier();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CScreamerShieldDamageNotifier();


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
		virtual void onActivate();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Resta los da�os indicados a la entidad (tanto de escudo
		como de vida), dispara los sonidos de da�o correspondientes
		y envia el mensaje de muerte si es necesario.

		@param damage Da�o que queremos restar al personaje.
		@param enemy Entidad que nos est� haciendo da�o.
		*/
		void damaged(int damage, CEntity* enemy);

		//__________________________________________________________________

		/**
		Dado un porcentaje de reducci�n de da�os (comprendido entre
		0 y 1) reduce todo el da�o recibido en base a ese porcentaje.

		Por ejemplo, un porcentaje de 1 har�a que el personaje siempre
		recibiera un da�o de 0 puntos (o en otras palabras, que
		fuera inmune).

		@param percentage Porcentaje de reducci�n de da�os. Tiene que 
		estar comprendido entre 0 y 1.
		*/
		inline void reducedDamageAbsorption(float percentage);

		//__________________________________________________________________

		/**
		Este m�todo debe ser usado por aquella entidad que cree a esta entidad,
		para indicar que �sta �ltima le pertenece.

		@param owner Entidad l�gica que gener� a esta entidad.
		*/
		void setOwner(Logic::CEntity* owner);

	protected:

		virtual void onStart();

		virtual void onFixedTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** 
		Porcentaje de da�os recibidos. Su valor debe estar entre 0 (el da�o no se
		reduce) y 1 (no recibimos da�os).
		*/
		float _reducedDamageAbsorption;

		/** Entidad due�a de esta entidad. */
		CEntity* _owner;

		CPhysicDynamicEntity* _physicComponent;

	}; // class CScreamerShieldDamageNotifier

	REG_FACTORY(CScreamerShieldDamageNotifier);

} // namespace Logic

#endif // __Logic_Life_H
