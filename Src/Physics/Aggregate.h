//---------------------------------------------------------------------------
// Aggregate.h
//---------------------------------------------------------------------------

/**
@file Aggregate.h



@see Physics::CAggregate

@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Physics_Aggregate_H
#define __Physics_Aggregate_H

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
	class CDynamicEntity;
}

namespace Logic {
	class IPhysics;
}

namespace Physics {

	/**
	
	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CAggregate {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CAggregate::CAggregate();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CAggregate();


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
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component, bool nameActors = false);

		//________________________________________________________________________

		/** Activa la simulaci�n f�sica de todos los actores asociados al agregado. */
		void activateSimulation();

		//________________________________________________________________________

		/** Desactiva la simulaci�n f�sica de todos los actores asociados al agregado. */
		void deactivateSimulation();

		//________________________________________________________________________

		/** 
		Devuelve un vector que contiene los punteros a las entidades fisicas
		que pertenecen al agregado.
		
		@return Array de punteros de las entidades f�sicas que componen el agregado.
		*/
		const std::vector<Physics::CDynamicEntity*> getEntities();


	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


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


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Agregado que contiene todos los actores leidos desde fichero (si es que hay m�s de uno). */
		physx::PxAggregate* _aggregate;

		/** Vector que contiene el puntero hacia los actores */
		std::vector<Physics::CDynamicEntity*> _actors;

	}; // class CAggregate

} // namespace Physics

#endif // __Physics_Aggregate_H
