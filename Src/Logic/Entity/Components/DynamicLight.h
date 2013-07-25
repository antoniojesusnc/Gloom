/**
@file DynamicLight.h

Contiene la declaraci�n del componente 
que encapsula el manejo de luces que siguen
a la entidad.

@see Logic::CDynamicLight
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Logic_DynamicLight_H
#define __Logic_DynamicLight_H

#include "Logic/Entity/Component.h"
#include "Light.h"

namespace Logic {

	/**
	Componente que encapsula el uso de luces y adem�s toma constantemente
	la posici�n y orientaci�n de la entidad a la que pertenece.
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Julio, 2013
	*/

	class CDynamicLight : public CLight {
		DEC_FACTORY(CDynamicLight);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CDynamicLight();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CDynamicLight();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		No acepta ning�n tipo de mensajes. Sobreescrito para evitar el comportamiento
		de la clase padre.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message) { return false; }


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Crea una luz con los par�metros leidos de mapa o establecidos manualmente.

		@param offset Offset que queremos que la luz tenga sobre la posici�n de la
		entidad.
		@param timeToLive Indica el tiempo que queremos que la luz est� encendida.
		Si el valor es 0 se asume que no queremos apagar la luz (por lo menos de forma
		autom�tica con un contador).
		*/
		void turnOn(const Vector3& offset = Vector3::ZERO, float timeToLive = 0.0f);


	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Actualiza la posici�n y orientaci�n de la luz constantemente.
		*/
		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Controla el tiempo que la luz est� encendida. */
		float _timer;
		
		/** Offset sobre la posici�n de la entidad. */
		Vector3 _offset;
	}; // class CDynamicLight

	REG_FACTORY(CDynamicLight);

} // namespace Logic

#endif // __Logic_Light_H
