//---------------------------------------------------------------------------
// CharacterController.h
//---------------------------------------------------------------------------

/**
@file CharacterController.h

Contiene la declaraci�n de la clase que encapsula al controlador de PhysX.

@see Graphics::CCharacterController
@see physx::PxController

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_CharacterController_H
#define __Physics_CharacterController_H

#include "BaseSubsystems/Math.h"
#include <vector>

#include <characterkinematic/PxController.h>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace physx {
	class PxCapsuleController;
	class PxControllerManager;
	class PxScene;
	class PxPhysics;
	class PxRigidActor;
	class PxAggregate;
	class PxCooking;
}

namespace Physics {
	class CCollisionManager;
}

namespace Logic {
	class CPhysicController;
	class IPhysics;
}

namespace Physics {

	// Flags que controlan con que colisiona el controller
	enum CharacterControllerFlag {
		// El controller esta colisionando por los lados
		eCOLLISION_SIDES	= physx::PxControllerFlag::eCOLLISION_SIDES,
		// El controller colisiona por la parte superior
		eCOLLISION_UP		= physx::PxControllerFlag::eCOLLISION_UP,
		// El controller colisiona por la parte inferior
		eCOLLISION_DOWN		= physx::PxControllerFlag::eCOLLISION_DOWN,
	};

	/**
	El objetivo de esta clase es abstraer el comportamiento de los character
	controllers. Por debajo se apoya en el uso del controlador de PhysX 
	(PxController).

	Notar que la c�psula f�sica del jugador no gira en PhysX, tan solo se mueve.
	Por este motivo no podemos ni obtener ni fijar la orientaci�n de la c�psula,
	tan solo la posici�n.

	Es posible aplicar una serie de filtros al controlador que luego influyen
	en el movimiento kinem�tico y dem�s. De momento no se usa.
	
	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CCharacterController {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/** Constructor por defecto. */
		CCharacterController();

		//__________________________________________________________________

		/** Destructor. */
		~CCharacterController();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Deserializa los datos de un fichero y los asigna al controlador del jugador.

		Este m�todo deber�a ser utilizado para cargar ragdolls o hitboxes.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		*/
		void loadRagdoll(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);

		//__________________________________________________________________

		/**
		M�todo que se encarga de inicializar por completo el controlador. El
		constructor realmente no hace nada.

		@param position Posici�n donde queremos situar la c�psula.
		@param radius Radio de la c�psula.
		@param height Altura de la c�psula.
		@param group Grupo de colisi�n al que pertenece la c�psula.
		@param groupList Grupos de colisi�n con los que queremos que la
		c�psula interactue.
		@param component Puntero al componente l�gico asociado al controlador
		de la c�psula.
		*/
		void load(const Vector3 &position, float radius, float height, 
				  int group, const std::vector<int>& groupList, 
	              const Logic::CPhysicController* component);

		//__________________________________________________________________

		/**
		Mueve a la c�psula en la direcci�n que especifiquemos.

		@param movement Vector que indica la direcci�n en la que nos queremos
		mover.
		@param msecs Cantidad de milisegundos que queremos que se produzca el
		desplazamiento.
		@return Flags de colisi�n del controller.
		*/
		unsigned move(const Vector3 &movement, unsigned int msecs);

		//__________________________________________________________________

		/**
		Mueve a la c�psula en la direcci�n que especifiquemos con un filtro especifico.
		El filtro indicara contra que cosas queremos colisionar.

		@param movement Vector que indica la direcci�n en la que nos queremos
		mover.
		@param customFilterMask M�scara que contiene los grupos de colision contra los
		que queremos colisionar en este move.
		@param msecs Cantidad de milisegundos que queremos que se produzca el
		desplazamiento.
		@return Flags de colisi�n del controller.
		*/
		unsigned move(const Vector3& movement, unsigned int customFilterMask, unsigned int msecs);

		//__________________________________________________________________
		
		/**
		Devuelve la posici�n del controlador.

		@return La posici�n de la c�psula.
		*/
		Vector3 getPosition();

		//__________________________________________________________________

		/**
		Setea la posici�n del controlador. Ojo con este m�todo, ya que transporta
		a la c�psula de un sitio a otro.

		@param position Posicion en la que queremos situar la c�psula.
		*/
		void setPosition(const Vector3 &position);

		//__________________________________________________________________

		/**
		Devuelve la m�scara utilizada para el filtrado de colisiones.

		@return La m�scara utilizada para el filtrado de colisiones.
		*/
		unsigned int getFilterMask() { return _filterMask; }

		//__________________________________________________________________

		float getCapsuleRadius();

		//__________________________________________________________________

		float getCapsuleHeight();

		//__________________________________________________________________

		/** Activa la simulaci�n f�sica de la c�psula. */
		void activateSimulation();

		//__________________________________________________________________

		/** Desactiva la simulaci�n f�sica de la c�psula. */
		void deactivateSimulation();

	private:


		// =======================================================================
		//                          M�TODOS PRIVADOS
		// =======================================================================


		/** 
		M�todo para construir un actor de PhysX a partir de un fichero RepX.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		*/
		physx::PxRigidActor* deserializeFromRepXFile(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Controlador de la c�psula del controller. */
		physx::PxCapsuleController* _controller;

		/** Agregado que contiene todos los actores y articulaciones que pertenecen al ragdoll */
		physx::PxAggregate* _ragdoll;

		/** Gestor de controladores en PhysX. */
		physx::PxControllerManager* _controllerManager;

		/** Puntero a la escena f�sica. */
		physx::PxScene* _scene;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physxSDK;

		/** Puntero al gestor de colisiones */
		CCollisionManager* _collisionManager;

		/** Puntero al cocinado de PhysX. */
		physx::PxCooking* _cooking;

		/** Mascara de filtros asignados al controlador de capsula. */
		physx::PxU32 _filterMask;

	}; // class CCharacterController

} // namespace Physics

#endif // __Physics_CharacterController_H
