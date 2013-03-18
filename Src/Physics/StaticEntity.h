//---------------------------------------------------------------------------
// StaticEntity.h
//---------------------------------------------------------------------------

/**
@file StaticEntity.h

@see Physics::CStaticEntity

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_StaticEntity_H
#define __Physics_StaticEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace physx {
	class PxScene;
	class PxPhysics;
	class PxRigidActor;
}

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
}

namespace Logic {
	class CPhysicEntity;
	class IPhysics;
}

namespace Physics {

	/**
	
	LAS ENTIDADES FISICAS TIENEN ORIENTACION, NO SON COMO LOS CONTROLLERS!
	PERMITIR FIJAR ESTOS PARAMETROS

	De momento solo un shape por entidad, pero en el futuro un vector de shapes
	cada una con sus propiedades

	@ingroup physicsGroup

	Para los character controllers, las capsulas no giran, tan solo podemos obtener
	la posicion.

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CStaticEntity : public CEntity {
	public:

		void load(const Vector3 &position, const const physx::PxGeometry& geometry, physx::PxMaterial& material, 
				  bool trigger, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
