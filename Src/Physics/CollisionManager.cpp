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

// Para que se dispare tiene que estar contemplado en el filtro
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
	// Para cada pareja de actores en los cuales se ha detectado contacto
	// comprobamos el tipo de contacto que se ha detectado y disparamos los
	// metodos correspondientes de IPhysics
	for(PxU32 i =0 ; i < nbPairs; ++i) {
		const PxContactPair& cp = pairs[i];

		if( cp.flags & (PxContactPairFlag::eDELETED_SHAPE_0 | PxContactPairFlag::eDELETED_SHAPE_0) )
			continue;

		// Comprobamos si estamos tocando o no el objeto
		bool enter = cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST;

		// S�lo tenemos en cuenta los eventos de entrada y salida del contact. En PhysX 2.8 y anteriores
		// tambi�n se notificada si el objeto permanec�a en contacto. La documentaci�n no deja muy
		// claro si este comportamiento se ha eliminado de manera definitiva.
		if (!enter && !exit)
			continue;

		// Obtenemos los datos logicos asociados al primer actor
		IPhysics* firstActorBeingContacted = (IPhysics *) pairHeader.actors[0]->userData;
		assert(firstActorBeingContacted);

		// Obtenemos los datos logicos asociados al segundo actor
		IPhysics* secondActorBeingContacted = (IPhysics *) pairHeader.actors[1]->userData;
		assert(secondActorBeingContacted);

		// Disparamos los metodos onContact de la interfaz logica
		firstActorBeingContacted->onContact(secondActorBeingContacted,enter);
		secondActorBeingContacted->onContact(firstActorBeingContacted,enter);
		
	}
}

//--------------------------------------------------

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count) {
	// Recorrer el array de colisiones
	for (unsigned int i = 0; i < count; ++i) {
		
		// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		if( pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER) )
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
	
	
	CPhysicEntity* otherComponent = (CPhysicEntity *)hit.shape->getActor().userData;
	otherComponent->onShapeHit(hit); 
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

