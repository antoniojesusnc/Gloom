/**
@file FloatingMovement.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Octubre, 2010
*/
#ifndef __Logic_FloatingMovement_H
#define __Logic_FloatingMovement_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
*/
	class CFloatingMovement : public IComponent
	{
		DEC_FACTORY(CFloatingMovement);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CFloatingMovement() : IComponent(), _orientation(0), _currentOrbitalPos(0) {}

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		void estimateItemFloatingPos(Vector3& position, unsigned int msecs);

		void estimateItemRotation(unsigned int msecs);

	protected:
		// En radianes
		float _currentOrbitalPos;

		float _orbitalOffset;

		float _orbitalSpeed;

		float _orbitalRotationSpeed;

		float _orientation;

	}; // class CFloatingMovement

	REG_FACTORY(CFloatingMovement);

} // namespace Logic

#endif // __Logic_Life_H
