/**
@file ContactPoint.h

Contiene la declaraci�n del struct que contiene
informaci�n sobre el punto de contacto entre
dos shapes.

@see Logic::CContactPoint

@author Francisco Aisa Garc�a.
@date Abril, 2013.
*/

#ifndef __Physics_ContactPoint_H
#define __Physics_ContactPoint_H

#include "Conversions.h"
#include "BaseSubsystems/Math.h"

namespace Logic {
	class CEntity;
	class IPhysics;
}

namespace Physics {

	/**
	Este struct condensa los atributos m�s relevantes
	cuando se produce un impacto entre dos shapes.

	@author Francisco Aisa Garc�a
	@date Abril, 2013
	*/

	struct CContactPoint {
		/** Impulso aplicado en el punto de impacto. Divido por el deltaTime es la fuerza que se aplica. */
		Vector3 impulse;
		/** Posicion del impacto. */
		Vector3 position;
		/** Normal del impacto */
		Vector3 normal;
		/** Separaci�n entre shapes. Separaci�n negativa indica solapamiento de shapes. */
		float separation;

		CContactPoint() : impulse(Vector3::ZERO), position(Vector3::ZERO), normal(Vector3::ZERO), separation(0.0f) { }
	}; 

} // namespace Physics

#endif 
