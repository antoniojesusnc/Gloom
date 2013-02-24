/**
@file CollisionManager.cpp

Contiene la implementaci�n del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "CollisionManager.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Logic/Entity/Entity.h"

#include <PxRigidActor.h>
#include <PxShape.h> 
#include <PxSimulationEventCallback.h> 
#include <PxPhysicsAPI.h>

using namespace Physics;
using namespace Logic;
using namespace physx;

//--------------------------------------------------

CCollisionManager::CCollisionManager() {

}

//--------------------------------------------------

CCollisionManager::~CCollisionManager() {

}

//--------------------------------------------------

void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onWake(PxActor **actors, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onSleep(PxActor **actors, PxU32 count) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {
	// Por ahora ignoramos estos mensajes

}

//--------------------------------------------------

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count) {
	// Recorrer el array de colisiones
	for (unsigned int i = 0; i < count; ++i) {
		
		// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		// Comprobamos si estamos saliendo o entrando en el trigger
		bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

		// S�lo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
		// tambi�n se notificada si el objeto permanec�a dentro del trigger. La documentaci�n no deja muy
		// claro si este comportamiento se ha eliminado de manera definitiva.
		if (!enter && !exit)
			continue;

		// Notificar a los componentes f�sicos de la l�gica las colisiones
		// 1. Obtener el componente l�gico (IPhysics) asociado al trigger f�sico.
		// 2. Obtener el componente l�gico (IPhysics) asociado a la otra entidad f�sica.
		// 3. Notificar a ambos componentes la colisi�n
		IPhysics *triggerComponent = (IPhysics *) pairs[i].triggerShape->getActor().userData;
		assert(triggerComponent);

		IPhysics *otherComponent = (IPhysics *) pairs[i].otherShape->getActor().userData;
		assert(otherComponent);

		triggerComponent->onTrigger(otherComponent, enter);
		otherComponent->onTrigger(triggerComponent, enter);
	}	
}

//--------------------------------------------------

void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit) {
	// TODO: notificar al componente f�sico la colisi�n con una entidad
	// 1. Obtener el puntero al componente f�sico (CPhysicController)
	// 2. Notificar la colisi�n al componente f�sico
	CPhysicController * component = (CPhysicController *) hit.controller->getUserData();
	component->onShapeHit(hit);
}

//--------------------------------------------------

void CCollisionManager::onControllerHit(const PxControllersHit &hit) {
	// TODO: notificar al componente f�sico la colisi�n con otro controller
	// 1. Obtener el puntero al componente f�sico (CPhysicController)
	// 2. Notificar la colisi�n al componente f�sico
	
}

//--------------------------------------------------

void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit) {
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

