//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que encapsula las operaciones 
b�sicas de los rigid bodies.

@see Physics::CEntity

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_Entity_H
#define __Physics_Entity_H

#include "BaseSubsystems/Math.h"

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
	class PxCooking;
	class PxCollection;
	class PxAggregate;
}

namespace Physics {
	class CCollisionManager;
}

namespace Logic {
	class IPhysics;
}

namespace Physics {

	/**
	Esta clase implementa la funcionalidad b�sica de los rigid bodies.
	Las entidades din�micas y est�ticas tendr�n que derivar de �sta.

	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CEntity {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CEntity::CEntity();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CEntity();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		M�todo virtual puro que debe ser implementado por las clases hijas para 
		deserializar los datos f�sicos desde un fichero.

		Contiene una implementaci�n por defecto que las clases hijas pueden 
		reutilizar.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		@param nameActors true si queremos que los colliders tengan un nombre asignado.
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component, bool nameActors = false) = 0;

		//________________________________________________________________________

		/** Devuelve la posici�n y rotaci�n de la entidad f�sica. */
		Matrix4 getTransform() const;

		//________________________________________________________________________

		/** Devuelve la posici�n de la entidad fisica*/
		Vector3 getPosition() const;

		//________________________________________________________________________

		/** Devuelve la rotaci�n de la entidad f�sica. */
		Quaternion getOrientation() const;

		//________________________________________________________________________

		/** Activa la simulaci�n f�sica. */
		void activateSimulation();

		//________________________________________________________________________

		/** Desactiva la simulaci�n f�sica. */
		void deactivateSimulation();


	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/** 
		M�todo para construir un actor de PhysX a partir de un fichero RepX. Si existen
		varios actores en el fichero se cargar�n como un agregado (se asume que todos los
		actores ser�n usados en una misma entidad).

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		@param nameActors true si queremos que se le asigne nombres a los colliders.
		*/
		void deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component,
									 bool nameActors);

		//________________________________________________________________________

		/**
		Dada una colecci�n de elementos, deserializa todos sus actores en un agregado.

		Normalmente usaremos esta funci�n para deserializar ragdolls.

		@param sceneCollection Colecci�n de elementos.
		@param nbActors N�mero de actores en la colecci�n.
		@param component Componente l�gico asociado a cada uno de los actores del agregado.
		@param group Grupo de colisi�n del agregado.
		@param Grupos de colisi�n con los que el agregado debe interactuar.
		*/
		void deserializeAggregate(physx::PxCollection* sceneCollection, unsigned int nbActors, const Logic::IPhysics* component, int group, const std::vector<int>& groupList);

		//________________________________________________________________________

		/**
		Dada una colecci�n de elementos, deserializa un solo actor. Se usa cuando se sabe
		que solo existe un actor en el fichero que vamos a deserializar.

		@param sceneCollection Colecci�n de elementos.
		@param component Componente l�gico asociado al actor que se va a deserializar.
		@param group Grupo de colisi�n del actor.
		@param Grupos de colisi�n con los que el actor debe interactuar.
		*/
		void deserializeActor(physx::PxCollection* sceneCollection, const Logic::IPhysics* component, int group, const std::vector<int>& groupList);

		//________________________________________________________________________

		/**
		Dado un actor, se activa su simulaci�n f�sica.

		@param actor Actor que va a ser activado.
		*/
		void activateSimulation(physx::PxActor* actor);

		//________________________________________________________________________

		/**
		Dado un actor, se desactiva su simulaci�n f�sica.

		@param actor Actor que va a ser desactivado.
		*/
		void deactivateSimulation(physx::PxActor* actor);

		//________________________________________________________________________

		/**
		Dada una geometr�a devuelve el desfase que existe entre el pivote l�gico y
		el f�sico, ya que en la f�sica el pivote se encuentra en el centro del objeto
		y en la l�gica en el pie.

		@param geometry Geometria a la cual queremos calcularle el desfase del pivote.
		@return Defase entre el pivote l�gico y el f�sico (solo afecta a la coordenada
		y).
		*/
		float getLogicPivotOffset(const physx::PxGeometry& geometry);


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Puntero al actor de PhysX. Importante: Tiene que ser inicializado por la clase hija. */
		physx::PxRigidActor* _actor;

		/** Agregado que contiene todos los actores leidos desde fichero (si es que hay m�s de uno). */
		physx::PxAggregate* _aggregate;

		/** Puntero a la escena de PhysX. */
		physx::PxScene* _scene;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physxSDK;

		/** Puntero al cocinado de PhysX. */
		physx::PxCooking* _cooking;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

		/** True si el actor representa a un trigger. */
		bool _isTrigger;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
