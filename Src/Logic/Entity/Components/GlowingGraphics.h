/**
@file GlowingGraphics.h

@see Logic::CGlowingGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Logic_GlowingGraphics_H
#define __Logic_GlowingGraphics_H

#include "Logic/Entity/Component.h"
#include "AnimatedGraphics.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics {
	class CEntity;
	class CGlowingEntity;
}

//declaraci�n de la clase
namespace Logic {

	/**

    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
	*/
	
	class CGlowingGraphics : public CAnimatedGraphics {
		DEC_FACTORY(CGlowingGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CGlowingGraphics();

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CGlowingGraphics();

	protected:

		/**
		M�todo virtual que construye la entidad gr�fica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gr�ficas (animadas, etc.).
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);
		
		/**
		Entidad gr�fica.
		*/
		Graphics::CGlowingEntity* _glowingGraphicsEntity;

	}; // class CGraphics

	REG_FACTORY(CGlowingGraphics);

} // namespace Logic

#endif // __Logic_GlowingGraphics_H
