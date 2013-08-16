/**
@file Light.h

Contiene la declaraci�n del componente 
que encapsula el manejo de luces.

@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Logic_Light_H
#define __Logic_Light_H

#include "Logic/Entity/Component.h"
#include "Graphics/Light.h"

namespace Logic {

	/**
	Componente que encapsula el uso de las luces.
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
	*/

	class CLight : public IComponent {
		DEC_FACTORY(CLight);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CLight();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CLight();


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
			<li><strong>lightType:</strong> Tipo de luz, focal, puntual o direccional. </li>
			<li><strong>controlledByManager:</strong> true si queremos que la luz la gestione el manager. </li>
			<li><strong>position:</strong> Posici�n de la luz. </li>
			<li><strong>on:</strong> true si queremos que la luz aparezca encendida de primeras. </li>
			<li><strong>direction:</strong> Direcci�n en la que queremos que apunte la luz (in�til para las luces puntuales). </li>
			<li><strong>color:</strong> Color de la luz. </li>
			<li><strong>attenuation:</strong> Atenuaci�n de la luz. </li>
			<li><strong>range:</strong> Rango de efecto de la luz. Si vale 0, se considera infinito. </li>
			<li><strong>innerAngle:</strong> �ngulo del cono interior del spotlight. </li>
			<li><strong>outerAngle:</strong> �ngulo del cono exterior del spotlight. </li>
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
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Cambiar el color de la luz.

		@param color Color de la luz en formato r, g, b. Los valores var�an entre 0 y 1.
		*/
		void setColor(const Vector3& color);

		//________________________________________________________________________

		/**
		Atenuaci�n de la luz, donde la x es atenuaci�n constante, la y atenuaci�n linear
		y la z atenuaci�n cuadr�tica.

		@param attenuation Vector de atenuaci�n.
		*/
		void setAttenuation(const Vector3& attenuation);

		//________________________________________________________________________

		/**
		Rango de alcance de la luz. Ojo con este par�metro, ya que Ogre deja de renderizar la
		luz si no est� dentro del frustrum de c�mara y la distancia entre la luz y la entidad
		es mayor que el rango dado. Si ponemos mal el rango vamos a ver como se apaga
		la luz.

		@param range Rango de alcance de la luz.
		*/
		void setRange(float range);

		//________________________________________________________________________

		/**
		Configura las dimensiones del cono exterior e interior de un spotlight.
		Si la luz no es un spotlight no tiene ning�n efecto.

		@param innerAngle �ngulo del cono interior.
		@param outerAngle �ngulo del cono exterior.
		*/
		void setSpotLightParams(float innerAngle, float outerAngle);

		//________________________________________________________________________

		/**
		Cambia la posici�n de la luz a la posici�n dada.

		@param position Posici�n en la que queremos situar la luz.
		*/
		void setPosition(const Vector3& position);

		//________________________________________________________________________

		/**
		Cambia la orientaci�nd de la luz.

		@param rotation Orientaci�n que queremos darle a la luz.
		*/
		void setOrientation(const Quaternion& rotation);

		//________________________________________________________________________

		/**
		Cambia la direcci�n en la que apunta la luz.

		@param direction Direcci�n en la que queremos que apunte la luz.
		*/
		void setDirection(const Vector3& direction);

		//________________________________________________________________________

		/**
		Destruye la luz, no la apaga. Ojito con llamar muy a menudo a este
		m�todo.
		*/
		void turnOff();


	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Dependiendo de si en el mapa se ha especificado que la luz este encendida o
		apagada, al ejecutarse este m�todo se enciende la luz o se deja apagada.
		*/
		virtual void onStart();


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Componente gr�fico de luz. */
		Graphics::CLight* _light;

		/** Tipo de la luz. */
		Graphics::LightType::Enum _lightType;

		/** Posici�n de la luz. */
		Vector3 _position;

		/** Direcci�n en la que apunta la luz. In�til para luces puntuales. */
		Vector3 _direction;

		/** Color de la luz. x = r, y = g, z = b. */
		Vector3 _color;

		/** Atenuaci�n de la luz. x = constante, y = linear, z = cuadr�tica. */
		Vector3 _attenuation;

		/** Grupo al que pertenece la luz. */
		unsigned int _lightGroup;

		/** �ngulo del cono interior del spotlight. Solo �til para spotlights. */
		float _innerAngle;

		/** �ngulo del cono exterior del spotlight. Solo �til para spotlights. */
		float _outerAngle;

		/** Rango de alcance de la luz. */
		float _range;

		/** true si queremos que la luz se cree encendida. */
		bool _isOn;

		/** true si queremos que el manager gestione la creaci�n/destrucci�n de �sta luz. */
		bool _controlledByManager;

		/** true si la luz es estatica, es decir, si ha sido precocinada en el lightmap. */
		bool _static;
	}; // class CLight

	REG_FACTORY(CLight);

} // namespace Logic

#endif // __Logic_Light_H
