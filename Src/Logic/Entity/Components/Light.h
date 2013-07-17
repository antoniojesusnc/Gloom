/**
@file Light.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#ifndef __Logic_Light_H
#define __Logic_Light_H

#include "Logic/Entity/Component.h"

namespace Graphics {
	class CLight;
}

//declaraci�n de la clase
namespace Logic {

	/**
	Este componente controla la luz que tiene un objeto. 
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
	*/

	class CLight : public IComponent {
		DEC_FACTORY(CLight);
	public:

		/** Constructor por defecto.*/
		CLight();

		/** Destructor. */
		virtual ~CLight();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:

		virtual void onStart();

		//virtual void onTick(unsigned int msecs);

		/**
		Luz
		*/
		Graphics::CLight* _light;

		Vector3 _position;
		Vector3 _direction;
		Vector3 _color;
		Vector3 _attenuation;
		float _innerAngle;
		float _outerAngle;

	}; // class CLight

	REG_FACTORY(CLight);

} // namespace Logic

#endif // __Logic_Light_H
