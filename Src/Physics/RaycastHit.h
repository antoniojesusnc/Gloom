/**
@file RaycastHit.h

Contiene la declaraci�n del struct que contiene
informaci�n del hit de un raycast de f�sicas.

@see Logic::CRaycastHit

@author Francisco Aisa Garc�a.
@date Abril, 2013.
*/

#ifndef __Physics_RaycastHit_H
#define __Physics_RaycastHit_H

#include "BaseSubsystems/Math.h"

namespace Logic {
	class CEntity;
}

namespace Physics {

	/**
	Este struct condensa los atributos m�s relevantes
	devueltos por la query de raycast de f�sicas.

	@author Francisco Aisa Garc�a
	@date Abril, 2013
	*/

	struct CRaycastHit {
		/** Puntero a la entidad golpeada */
		Logic::CEntity* entity;
		/** Distancia desde el punto de salida del ray hasta el golpeo. */
		float distance;
		/** Punto de impacto del raycast. */
		Vector3 impact;
		/** Normal de impacto del raycast. */
		Vector3 normal;

		CRaycastHit(): entity(0), distance(-1), impact( Vector3::ZERO ), normal( Vector3::ZERO ) {};
	}; 

} // namespace Physics

#endif 
