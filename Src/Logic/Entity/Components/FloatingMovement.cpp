/**
@file FloatingMovement.cpp

@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#include "FloatingMovement.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include <math.h>

namespace Logic {
	
	IMP_FACTORY(CFloatingMovement);
	
	//________________________________________________________________________

	Vector3 CFloatingMovement::estimateItemFloatingPos(const Vector3& position, unsigned int msecs) {
		// La velocidad a la que el item recorre la funcion seno depende de _orbitalSpeed
		// Si llegamos 2Pi, reseteamos el valor a cero.
		_currentOrbitalPos += _orbitalSpeed * msecs;
		if(_currentOrbitalPos > 6.283) _currentOrbitalPos = 0;

		// Calculamos la posicion de la vertical en funcion del valor obtenido al calcular el seno.
		// Multiplicamos el resultado por el offset para conseguir que el objeto recorra mas distancia
		// pero a la misma velocidad.
		return Vector3(position.x, position.y + (sin(_currentOrbitalPos) * _orbitalOffset), position.z);
	}

	//________________________________________________________________________

	float CFloatingMovement::estimateItemRotation(unsigned int msecs) {
		// Giramos en funcion de la velocidad de giro.
		return _entity->getYaw() + (_orbitalRotationSpeed * msecs);
	}

	//________________________________________________________________________

	void CFloatingMovement::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		
		// Calculamos la nueva posicion de la vertical del item
		_entity->setPosition( estimateItemFloatingPos(_entity->getPosition(), msecs) );
		// Calculamos la nueva orientacion del item
		_entity->setYaw( estimateItemRotation(msecs) );
		
	} // tick

	//________________________________________________________________________

	bool CFloatingMovement::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("orbitalSpeed")) {
			_orbitalSpeed = entityInfo->getFloatAttribute("orbitalSpeed");
		}

		if(entityInfo->hasAttribute("orbitalOffset")) {
			_orbitalOffset = entityInfo->getFloatAttribute("orbitalOffset");
		}

		if(entityInfo->hasAttribute("orbitalRotationSpeed")) {
			_orbitalRotationSpeed = entityInfo->getFloatAttribute("orbitalRotationSpeed");
		}

		return true;
	} // spawn


} // namespace Logic

