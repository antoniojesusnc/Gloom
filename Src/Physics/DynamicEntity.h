//---------------------------------------------------------------------------
// DynamicEntity.h
//---------------------------------------------------------------------------

/**
@file DynamicEntity.h

Contiene la declaraci�n de la clase que representa a las entidades f�sicas
din�micas.

@see Physics::CDynamicEntity

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_DynamicEntity_H
#define __Physics_DynamicEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"

#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
#include <PxForceMode.h>

#include <vector>
#include <string>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace physx {
	class PxRigidDynamic;
}

namespace Physics {

	// Enumerado para los tipos de fuerza que se le pueden aplicar
	// a una entidad din�mica.
	enum ForceMode {
		eFORCE				= physx::PxForceMode::eFORCE,
		eIMPULSE			= physx::PxForceMode::eIMPULSE,
		eVELOCITY_CHANGE	= physx::PxForceMode::eVELOCITY_CHANGE,
		eACCELERATION		= physx::PxForceMode::eACCELERATION
	};

	/**
	Clase que representa a las entidades f�sicas din�micas.

	IMPORTANTE: Las entidades f�sicas tienen orientaci�n (a diferencia de
	los controllers). Actualmente solo soportamos m�todos de seteo de posici�n.
	Deberiamos permitir setear la orientaci�n tambi�n. Adem�s de esto, solo
	permitimos la carga de un shape por actor, esto deber�a cambiar en el futuro.

	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CDynamicEntity : public CEntity {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CDynamicEntity::CDynamicEntity();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CDynamicEntity();


		// =======================================================================
		//                     METODOS HEREDADOS DE CEntity
		// =======================================================================


		/**
		Carga una entidad f�sica desde un fichero RepX.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		M�todo que inicializa la entidad f�sica. Actualmente asignamos por defecto
		una sola shape a la entidad (definida por la geometr�a y densidad dadas).
		En realidad, una entidad f�sica puede estar formada por varias shapes, por
		lo que es previsible que en el futuro cambiemos este m�todo para que sea
		a�n m�s gen�rico.

		@param position Posici�n donde queremos crear la entidad f�sica.
		@param geometry Geometr�a que queremos que tenga la entidad f�sica. 
		Importante: PhysX solo soporta ciertas geometrias para los din�micos.
		@param material Material f�sico que aplicaremos a la entidad.
		@param density Densidad de la geometr�a que vamos a asignar a la entidad.
		@param kinematic True si la entidad es kinem�tica.
		@param trigger True si la entidad es un trigger.
		@param group Grupo de colisi�n del actor.
		@param groupList Grupos de colisi�n con los que queremos que la entidad interactue.
		@param component Componente l�gico asociado.
		*/
		void load(const Vector3 &position, const physx::PxGeometry& geometry, physx::PxMaterial& material, 
				  float density, bool kinematic, bool trigger, int group, 
				  const std::vector<int>& groupList, const Logic::IPhysics* component);

		//________________________________________________________________________
						 
		/**
		Empuja a la entidad f�sica en una direcci�n, con una fuerza y de una cierta manera.

		@param forceVector Vector de fuerza, t�picamente = vectorDirector * magnitudFuerza
		@param forceMode Modo de empuje, por defecto eFORCE (empuje lineal).
		@param autowake True si queremos que el actor se despierte al ser empujado (si estaba
		durmiendo).
		*/
		void addForce(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		//________________________________________________________________________

		/**
		Empuje rotatorio en una direcci�n, con una fuerza y de una cierta manera.

		@param forceVector Vector de fuerza, t�picamente = vectorDirector * magnitudFuerza
		@param forceMode Modo de empuje, por defecto eFORCE (empuje lineal).
		@param autowake True si queremos que el actor se despierte al ser empujado (si estaba
		durmiendo).
		*/
		void addTorque(const Vector3& forceVector, ForceMode forceMode = ForceMode::eFORCE, bool autowake = true);

		//________________________________________________________________________

		/**
		Desactiva la gravedad para esta entidad.

		IMPORTANTE: Si ejecutamos este m�todo durante la simulaci�n f�sica, no se
		despertar�n los actores a los que afecte este cambio (por razones de 
		eficiencia seg�n PhysX). En ese caso tenemos que despertarlos de forma
		manual.

		@param state True si queremos desactivar la gravedad.
		*/
		void disableGravity(bool state);

		//________________________________________________________________________

		/**
		M�todo para saber si la entidad f�sica es kinem�tica.

		@return True si la entidad es kinem�tica.
		*/
		bool isKinematic();

		//________________________________________________________________________

		/**
		M�todo para mover a la entidad f�sica.

		IMPORTANTE: Para poder mover el actor �ste debe ser kinem�tico, en caso contrario
		no funciona.

		@param transform Posici�n y orientaci�n objetivo que queremos tener.
		*/
		void move(const Matrix4 &transform);

		//________________________________________________________________________

		/**
		M�todo para mover a la entidad f�sica.

		IMPORTANTE: Para poder mover el actor �ste debe ser kinem�tico, en caso contrario
		no funciona.

		@param transform Posici�n objetivo que queremos tener.
		*/
		void move(const Vector3& displ);

		//________________________________________________________________________

		/** 
		Setea la posici�n de la entidad f�sica. Mucho cuidado porque este m�todo
		lo que hace es transportar a la entidad (no moverla).

		IMPORTANTE: Actualmente solo tenemos un set position, pero las entidades
		f�sica TIENEN orientaci�n. Ser�a �til tener un metodo de colocaci�n que
		oriente tambi�n a la entidad f�sica.

		@param position Posicion a donde queremos transportar la entidad.
		@param makeConversionToLogicWorld True si queremos que la entidad f�sica
		se coloque en la posici�n dada + el desfase entre pivote l�gico y f�sico.
		*/
		void setPosition(const Vector3 &position, bool makeConversionToLogicWorld);

		/** 
		Setea el transform de la entidad f�sica. Mucho cuidado porque este m�todo
		lo que hace es transportar a la entidad (no moverla).

		@param transform Transform al que queramos convertir la entidad la entidad.
		@param makeConversionToLogicWorld True si queremos que la entidad f�sica
		se coloque en la posici�n dada + el desfase entre pivote l�gico y f�sico.
		*/
		void setTransform(const Matrix4 &transform, bool makeConversionToLogicWorld);


		/** 
		Setea el transform de la entidad f�sica. Mucho cuidado porque este m�todo
		lo que hace es transportar a la entidad (no moverla).

		@param position Position del que queremos recalcular el pivote
		@return Posici�n nueva ponderada en la Y para el pivote
		*/
		Vector3 CDynamicEntity::TransformPositionLogicPhysX(Vector3 position);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Actor din�mico. */
		physx::PxRigidDynamic* _dynamicActor;

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
