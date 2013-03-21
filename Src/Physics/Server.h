/**
@file Server.h

Contiene la declaraci�n del servidor de f�sica. 

@see Physics::CServer

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"
//#include <geometry/PxGeometry.h>
//#include "Physics/GeometryFactory.h"
//#include "Physics/MaterialManager.h"

//#include <PxForceMode.h>

// Predeclaraci�n de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;
	class PxRigidBody;
	class PxControllerBehaviorCallback;

	namespace pxtask {
		class CudaContextManager;
	};

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};



// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Servidor de f�sica. Se encarga de gestionar las entidades f�sicas y simular su 
	comportamiento. Cumple varias finalidades:
	
	<ul>
	<li> Proporciona una interfaz com�n con el resto de servidores de la aplicaci�n (gr�ficos, GUI, etc). </li> 
	<li> A�sla el resto de la aplicaci�n del motor de f�sica concreto que usamos (hasta cierto punto).
	<li> Realiza las conversiones entre los tipos l�gicos y los de PhysX (vectores, matrices, etc).
	</ul>
	<p>
	Las entidades f�sicas de PhysX y los componentes l�gicos que proporcionan la representaci�n f�sica de la entidad
	est�n relacionados de dos formas:
	<ul>
	<li> Los componentes l�gicos almacenan internamente sus actores de PhysX asociados. </li>
	<li> Usamos el atributo userData de los actores de PhysX para almacenar el componente l�gico asociado. </li>
	</ul>
	De esta forma, la capa l�gica pueden mover los objetos f�sicos (actores cinem�ticos, character controllers, 
	aplicar fuerzas a actores din�micos, etc) y la capa de f�sica puede comunicar las colisiones a los componentes
	l�gicos.
	<p>
	Es aconsejable utilizar los m�todos del servidor para cambiar / recuperar las posiciones de las
	entidades f�sicas, ya que el servidor realiza las conversiones necesarias entre sistemas de coordenadas.
	Aunque la l�gica y PhysX usan el mismo tipo de sistema de coordenadas, PhysX asume que el origen est� 
	en medio del objeto mientras que la l�gica asume que el origen est� a los pies del objeto. Usando los 
	m�todos que	proporciona el servidor, la l�gica no tiene que preocuparse de estas conversiones.
	<p>
	Esta clase est� implementada como un Singleton de inicializaci�n expl�cita: es necesario 
	invocar al m�todo Init() al principio de la aplicaci�n y al m�todo Release() al final.

	@ingroup physicGroup

	@author Antonio S�nchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CServer 
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor f�sico. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor f�sico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Realiza la simulaci�n f�sica y actualiza la posici�n de todas las 
		entidades f�sicas. 

		@param secs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.
		*/
		void createScene ();

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene ();

		/**
		Establece si debe haber colisiones entre dos grupos de colisi�n. Al crear los objetos
		f�sicos se establece el grupo de colisi�n al que pertenecen. Usando este m�todo es
		posible activar / desactivar las colisiones entre grupos.

		@param group1 Primer grupo de colisi�n.
		@param group2 Segundo grupo de colisi�n
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);


		//------------------------------
		// Gesti�n de entidades simples
		//------------------------------

		/**
		Elimina una entidad f�sica de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor f�sico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor *actor);

		
		//----------------------------------
		// Consultas 
		//----------------------------------

		/**
		 Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca. Si el rayo
		 no choca contra ninguna entidad devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia m�xima de la primera intersecci�n.
		 @return Primera entidad l�gica alcanzada o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const; 

		/**
		 Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca que pertenezca 
		 al grupo de colisi�n indicado. Si el rayo no choca contra ninguna entidad de ese grupo
		 devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia m�xima de la primera intersecci�n.
		 @param group Grupo de colisi�n de la entidad buscada.
		 @return Primera entidad l�gica alcanzada de ese grupo o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist, int group) const; 

		/**
		Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca que NO es la indicada en el ID.
		Si el rayo no choca contra ninguna entidad devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia m�xima de la primera intersecci�n.
		 @param id Id con la que no puede chocar.
		 @return Primera entidad l�gica alcanzada o NULL.
		 */
		Logic::CEntity* raycastClosestInverse (const Ray& ray, float maxDist, unsigned int id) const; 

		void overlapExplotion(const Vector3& position, float explotionRadius, Logic::CEntity** & entitiesHit, int& nbHits);

		//void overlapMultiple(const physx::PxGeometry& geometry, const Vector3& position, Logic::CEntity** & entitiesHit, int& nbHits);

		//void overlapAny(const physx::PxGeometry& geometry, const Vector3& position);

		void setupFiltering(physx::PxRigidActor* actor, int group, const std::vector<int>& groupList);

		physx::PxScene* getActiveScene() { return _scene; }

		physx::PxPhysics* getPhysxSDK() { return _physics; }

		physx::PxControllerManager* getControllerManager() { return _controllerManager; }

		physx::PxCooking* getCooking() { return _cooking; }

		CCollisionManager* getCollisionManager() { return _collisionManager; }

	private:	

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia �nica de la clase.
		static CServer *_instance;

		// Gestor de errores
		physx::PxErrorCallback *_errorManager;

		// Gestor de memoria
		physx::PxAllocatorCallback *_allocator;

		// Distintos atributos relacionados con PhysX
		physx::PxFoundation *_foundation;
		physx::PxProfileZoneManager *_profileZoneManager;
		physx::PxDefaultCpuDispatcher *_cpuDispatcher;
		physx::pxtask::CudaContextManager *_cudaContextManager;
		physx::debugger::comm::PvdConnection *_pvdConnection;
		physx::PxCooking *_cooking;

		// SDK de PhysX
		physx::PxPhysics *_physics;

		// Escena f�sica
		physx::PxScene *_scene;

		// Gestor de controller managers
		physx::PxControllerManager* _controllerManager;

		// Material que se aplica por defecto a las nuevas entidades f�sicas
		physx::PxMaterial *_defaultMaterial;

		// Gestion de colisiones
		CCollisionManager *_collisionManager;

		// Tiempo m�nimo de ejecuci�n de f�sica
		unsigned int _acumTime;

		unsigned int _fixedTime;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
