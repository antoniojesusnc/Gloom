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

		/** Constructor por par�metro */
		CEntity::CEntity(physx::PxRigidActor* adoptActor);

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
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component) = 0;

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

		//________________________________________________________________________

		/** 
		Devuelve el nombre del actor f�sico. OJO! Si el actor no se ha inicializado
		usando una stringTable explota.
		*/
		std::string getName();

		//________________________________________________________________________

		/**
		M�todo para saber si la entidad f�sica es din�mica.

		@return True si la entidad f�sica es din�mica.
		*/
		bool isDynamic();

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
		*/
		void deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);

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

		/** True si el actor representa a un trigger. �til solo si tenemos una �nica shape por actor. */
		bool _isTrigger;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
