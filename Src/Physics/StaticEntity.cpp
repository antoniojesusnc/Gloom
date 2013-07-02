//---------------------------------------------------------------------------
// StaticEntity.cpp
//---------------------------------------------------------------------------

/**
@file StaticEntity.cpp

Contiene la implementaci�n de la clase que representa a las entidades f�sicas
est�ticas.

@see Physics::CStaticEntity

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#include "StaticEntity.h"

#include <assert.h>

#include "Physics/Server.h"
#include "Physics/CollisionManager.h"
#include "Conversions.h"

#include "Logic/Entity/Components/Physics.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxShape.h>
#include <PxRigidActor.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxSimpleFactory.h>

using namespace physx;
using namespace std;

namespace Physics {

	void CStaticEntity::load(const string &file, int group, const vector<int>& groupList, const Logic::IPhysics* component) {
		// Cargamos desde fichero los datos del actor
		CEntity::load(file, group, groupList, component);
	}

	//________________________________________________________________________

	void CStaticEntity::load(const Vector3& position, const Quaternion& orientation, const Geometry& geometry, Material& material, 
					         bool trigger, int group, const vector<int>& groupList, const Logic::IPhysics* component) {

		// Obtenemos el puntero al servidor de fisicas
		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();
		PxScene* scene = physicsServer->getActiveScene();
		PxPhysics* physics = physicsServer->getPhysxSDK();
		assert(scene);

		_isTrigger = trigger;
						
		// Creamos Transform
		PxTransform globalPose(Vector3ToPxVec3(position),QuaternionToPxQuat(orientation));
		
		// Transformaci�n de coordenadas l�gicas a coodenadas de PhysX
		PxTransform shapeOffset( PxVec3(0, getLogicPivotOffset(geometry), 0) );
		
		_actor = PxCreateStatic(*physics, globalPose, geometry, material, shapeOffset);

		// Transformarlo en trigger si es necesario
		if (trigger) {
			PxShape *shape;
			_actor->getShapes(&shape, 1, 0);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Asociar el componente l�gico asociado a la entidad f�sica
		_actor->userData = (void*) component;

		// Establecer el grupo de colisi�n
		PxSetGroup(*_actor, group);
		// Establecer los filtros en base al grupo de colision
		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);

		// A�adir el actor a la escena
		scene->addActor(*_actor);
	}

	//________________________________________________________________________

	void CStaticEntity::load(const PlaneGeometry& plane, Material& material, int group, 
							 const vector<int>& groupList, const Logic::IPhysics* component) {

		// Obtenemos el puntero al servidor de fisicas
		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();
		PxScene* scene = physicsServer->getActiveScene();
		PxPhysics* physics = physicsServer->getPhysxSDK();
		assert(scene);

		// Crear un plano est�tico
		_actor = PxCreatePlane(*physics, plane, material);
	
		// Anotar el componente l�gico asociado a la entidad f�sica
		_actor->userData = (void*) component;

		// Establecer el grupo de colisi�n
		PxSetGroup(*_actor, group);

		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, group, groupList);
	
		// A�adir el actor a la escena
		scene->addActor(*_actor);
	}

} // namespace Physics
