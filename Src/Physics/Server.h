/**
@file Server.h

Contiene la declaraci�n del servidor de f�sica. 

@see Physics::CServer

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"

#include "GeometryFactory.h"
#include "MaterialManager.h"

#include <PxFiltering.h>

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

namespace Physics {

	/**
	Esta funci�n es la que realmente se encarga que ocurra la magia de las notificaciones.
	Solo seran notificados de colisiones y triggers aquellos actores cuyos grupos de colisi�n
	hayan sido incluidos en la lista de grupos de colisi�n del otro actor con el que colisionan.

	Las m�scaras y grupos de colisi�n se fijan en la funci�n setupFiltering.

	@param attributes0 Informaci�n general sobre el primer objeto.
	@param filterData0 Datos de filtro del primer objeto.
	@param attributes1 Informaci�n general sobre el segundo objeto.
	@param filterData1 Datos de filtro del segundo objeto.
	@param pairFlags Flags de la pareja.
	@param constantBlock Puntero al bloque de datos.
	@param constantBlockSize Tama�o del bloque de datos.
	@return Los flags que indican que tipo de notificaci�n debe dispararse.
	*/
	physx::PxFilterFlags customFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
											physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
											physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	//________________________________________________________________________

	/**
	Clase principal del proyecto de f�sicas que se encarga de la inicializaci�n,
	gesti�n y liberaci�n del motor de f�sicas.
	
	El resto de clases del proyecto se apoyan en la comunicaci�n con esta clase
	para interactuar con el motor.

	De cara a la l�gica, el servidor abstrae la inicializaci�n y gesti�n de PhysX, 
	sirviendo de puente para la comunicaci�n entre la f�sica y el resto de proyectos.

	Es importante notar que esta clase no se encarga del manejo de las entidades de PhysX,
	tan solo de su simulaci�n; cada uno de los distintos tipos de entidades f�sicas que la 
	l�gica puede utilizar se encuentran abstraidos en clases distintas que se comunican 
	con el servidor (por motivos de coherencia y modularidad).
	
	IMPORTANTE: Esta clase est� implementada como un Singleton de inicializaci�n expl�cita: es necesario 
	invocar al m�todo Init() al principio de la aplicaci�n y al m�todo Release() al final.

	@ingroup physicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CServer {
	public:


		// =======================================================================
		//           M�TODOS DE INICIALIZACI�N Y LIBERACI�N DE RECURSOS
		// =======================================================================


		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer* getSingletonPtr() { return _instance; }

		//________________________________________________________________________
		
		/**
		Inicializa el servidor f�sico. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera el servidor f�sico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		//________________________________________________________________________

		/**
		Elimina una entidad f�sica de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor f�sico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor* actor);


		// =======================================================================
		//								   TICK
		// =======================================================================


		/**
		Realiza la simulaci�n f�sica y actualiza la posici�n de todas las 
		entidades f�sicas.

		La simulaci�n f�sica se hacen en peque�os steps de tiempo para evitar
		que se produzcan situaciones inesperadas al haber ticks muy grandes.

		@param secs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);


		// =======================================================================
		//                 M�TODOS DE GESTI�N DE LA ESCENA F�SICA
		// =======================================================================


		/**
		Crea la escena f�sica. De momento asumimos que solo vamos a tener una escena
		f�sica.

		En caso contrario ser�a conveniente crear un gestor de escenas y abstraer
		la escena de PhysX en una nueva clase.
		*/
		void createScene();

		//________________________________________________________________________

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene();


		// =======================================================================
		//            M�TODOS PARA LA GESTI�N DE LOS GRUPOS DE COLISI�N
		// =======================================================================


		/**
		Establece si debe haber colisiones entre dos grupos de colisi�n. Al crear los objetos
		f�sicos se establece el grupo de colisi�n al que pertenecen. Usando este m�todo es
		posible activar / desactivar las colisiones entre grupos.

		IMPORTANTE: Activar los grupos de colisi�n solo funciona para entidades y controladores.
		Los triggers no est�n incluidos. Para desactivar triggers y notificaciones hay
		que hacer uso del filter shader.

		@param group1 Primer grupo de colisi�n.
		@param group2 Segundo grupo de colisi�n
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);

		//________________________________________________________________________
		
		/**
		Dado un actor y su grupo de colisi�n, establece con que grupos de colisi�n
		debe haber notificaciones. Es importante notar que para que dos actores sean
		notificados de una colisi�n o de la activaci�n/desactivaci�n de un trigger,
		los grupos de colisi�n de ambos deben estar incluidos en la lista de grupos
		con los que colisionan.

		Esta funci�n hace uso de m�scaras de bits. Dichas m�scaras se asignan en funci�n
		de los grupos de colisi�n asignados a las entidades. Para llevar a cabo
		esta tarea existe otra funci�n (customFilterShader) que se pasa como callback a
		PhysX.

		@param actor Actor de PhysX al que queremos configurar sus filtros.
		@param group Grupo de colisi�n del actor.
		@param groupList Grupos de colisi�n con los que queremos que el actor sea notificado
		en caso de colisi�n.
		*/
		void setupFiltering(physx::PxRigidActor* actor, int group, const std::vector<int>& groupList);

		
		// =======================================================================
		//                     QUERIES DE OVERLAP Y RAYCAST
		// =======================================================================


		/**
		Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca. Si el rayo
		no choca contra ninguna entidad devuelve NULL.
		 
		@param ray Rayo lanzado.
		@param maxDist distancia m�xima de la primera intersecci�n.
		@return Primera entidad l�gica alcanzada o NULL.
		*/
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const;

		//________________________________________________________________________

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

		//________________________________________________________________________

		/**
		Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca que NO es la indicada en el ID.
		Si el rayo no choca contra ninguna entidad devuelve NULL.
		 
		@param ray Rayo lanzado.
		@param maxDist distancia m�xima de la primera intersecci�n.
		@param id Id con la que no puede chocar.
		@return Primera entidad l�gica alcanzada o NULL.
		*/
		Logic::CEntity* raycastClosestInverse (const Ray& ray, float maxDist, unsigned int id) const;

		//________________________________________________________________________

		/**
		Dada una geometr�a, realiza una query de overlap y devuelve un vector con los punteros
		a las entidades que colisionan con dicha geometr�a.

		IMPORTANTE: Por motivos de eficiencia esta funci�n aplico un filtro a esta funci�n para que
		solo tenga en cuenta a las entidades din�micas.

		@param geometry Geometr�a para la query de overlap. Mirar la documentaci�n para ver cuales
		est�n soportadas.
		@param position Posici�n en la que queremos situar el centro de la geometr�a.
		@param entitiesHit Vector de entidades que colisionan con la geometr�a dada. Este vector
		NO DEBE SER INICIALIZADO!! ya que la propia funci�n se encarga de inicializarlo y rellenarlo.
		El cliente es el responsable de liberar la memoria reservada en el vector pasado. En caso
		de no haber colisiones, el n�mero de colisiones detectadas devuelto es 0 y por lo tanto
		no se reserva memoria (no hay que liberar memoria - pete asegurado si se libera).
		@param nbHits N�mero de entidades que colisionan contra la geometr�a dada, 0 en caso de
		no haber contacto con ninguna entidad.
		*/
		void overlapMultiple(const physx::PxGeometry& geometry, const Vector3& position, Logic::CEntity** & entitiesHit, int& nbHits);

		//________________________________________________________________________

		/**
		Dada una geometr�a, realiza una query de overlap y devuelve true si la geometr�a dada
		colisiona contra alg�n actor (din�mico o est�tico).

		Es mucho m�s eficiente que overlapMultiple. Usar cuando no nos interese saber contra
		qu� colisiona la geometr�a.

		@param geometry Geometr�a para la query de overlap. Mirar la documentaci�n para ver que geometr�as
		est�n soportadas.
		@param position Posici�n donde queremos colocar el centro de la geometr�a dada.
		@return True si existe colisi�n con alg�n actor, falso en caso contrario.
		*/
		bool overlapAny(const physx::PxGeometry& geometry, const Vector3& position);


		// =======================================================================
		//               M�TODOS PARA LA OBTENCI�N DE INFO DE PHYSX
		// =======================================================================


		/**
		Devuelve un puntero a la escena actual de PhysX.

		@return Puntero a la escena de PhysX.
		*/
		physx::PxScene* getActiveScene() const { return _scene; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al core de PhysX.

		@return Puntero al core de PhysX.
		*/
		physx::PxPhysics* getPhysxSDK() const { return _physics; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al gestor de controladores de PhysX.

		@return Puntero al gestor de controladores de PhysX.
		*/
		physx::PxControllerManager* getControllerManager() const { return _controllerManager; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al sistema de cocinado de PhysX.

		@return Puntero al sistema de cocinado de PhysX.
		*/
		physx::PxCooking* getCooking() const  { return _cooking; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al gestor de colisiones.

		@return Puntero al gestor de colisiones.
		*/
		CCollisionManager* getCollisionManager() const { return _collisionManager; }

	private:	


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase. */
		CServer();

		//________________________________________________________________________

		/** Destructor de la clase. */
		virtual ~CServer();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Instancia �nica de la clase (por ser singleton). */
		static CServer* _instance;

		/** Puntero al gestor de errores. */
		physx::PxErrorCallback* _errorManager;

		/** Puntero al gestor de memoria de PhysX. */
		physx::PxAllocatorCallback* _allocator;

		/** Foundation de PhysX. */
		physx::PxFoundation* _foundation;

		/** Profile zone manager. */
		physx::PxProfileZoneManager* _profileZoneManager;

		/** CPU dispatcher de PhysX. Puede ser sobreescrito. */
		physx::PxDefaultCpuDispatcher* _cpuDispatcher;
		
		/** Manejador del procesamiento de c�lculos en GPU. */
		physx::pxtask::CudaContextManager* _cudaContextManager;
		
		/** Para la gesti�n del debugger. */
		physx::debugger::comm::PvdConnection* _pvdConnection;
		
		/** Puntero al sistema de cocinado de PhysX. */
		physx::PxCooking* _cooking;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physics;

		/** Puntero a la escena activa de PhysX. */
		physx::PxScene* _scene;

		/** Puntero al gestor de controladores de PhysX. */
		physx::PxControllerManager* _controllerManager;

		/** Puntero al gestor de colisiones. */
		CCollisionManager* _collisionManager;

		/** Tiempo real acumulado. Sirve para comprobar si tenemos que realizar un o varios steps de simulaci�n f�sica. */
		unsigned int _acumTime;

		/** Tama�o del timestep que tomamos para realizar una simulaci�n. */
		unsigned int _fixedTime;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
