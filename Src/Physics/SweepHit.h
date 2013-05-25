/**
@file SweepHit.h

Contiene la declaraci�n del struct que contiene
informaci�n del hit de un sweep de f�sicas.

@see Logic::CSweepHit

@author Francisco Aisa Garc�a.
@date Abril, 2013.
*/

#ifndef __Physics_SweepHit_H
#define __Physics_SweepHit_H

#include "BaseSubsystems/Math.h"

namespace Logic {
	class CEntity;
}

namespace Physics {

	/**
	Este struct condensa los atributos m�s relevantes
	devueltos por la query de sweep de f�sicas.

	@author Francisco Aisa Garc�a
	@date Abril, 2013
	*/

	struct CSweepHit {
		/** Puntero a la entidad golpeada */
		Logic::CEntity* entity;
		/** Distancia desde el punto de salida del sweep hasta el golpeo. */
		float distance;
		/** Punto de impacto del sweep. */
		Vector3 impact;
		/** Normal de impacto del sweep. */
		Vector3 normal;
		
		CSweepHit(): entity(0), distance(-1), impact( Vector3::ZERO ), normal( Vector3::ZERO ) {};
	}; 

} // namespace Physics

#endif 
