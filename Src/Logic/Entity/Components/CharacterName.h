/**
@file CharacterName.h

@see Logic::CCharacterName
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Agosto, 2013
*/

#ifndef __Logic_CharacterName_H
#define __Logic_CharacterName_H

#include "Logic/Entity/Component.h"

namespace Graphics {
	class CMovable2dText;
}

namespace Logic {

	/**

    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Agosto, 2013
	*/

	class CCharacterName : public IComponent {
		DEC_FACTORY(CCharacterName);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CCharacterName();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CCharacterName();


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
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		void setVisible(bool isVisible);

		bool isVisible();

	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Dependiendo de si en el mapa se ha especificado que la luz este encendida o
		apagada, al ejecutarse este m�todo se enciende la luz o se deja apagada.
		*/
		virtual void onStart();

		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		Graphics::CMovable2dText* _text2d;

		float _size;
		Vector3 _offset;
		Vector3 _color;
		std::string _font;

	}; // class CCharacterName

	REG_FACTORY(CCharacterName);

} // namespace Logic

#endif // __Logic_CharacterName_H
