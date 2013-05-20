/**
@file PhysicDynamicEntity.h


@see Logic::CPhysicDynamicEntity
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_PhysicDynamicEntity_H
#define __Logic_PhysicDynamicEntity_H

#include "Physics.h"
#include "Physics/DynamicEntity.h"

#include <vector>

// Predeclaraci�n de tipos
namespace physx {
	class PxRigidActor;
	class PxControllerShapeHit;
};

namespace Physics {
	class CServer;
	class CGeometryFactory;
	class CMaterialManager;
};

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CPhysicDynamicEntity : public IPhysics {
		DEC_FACTORY(CPhysicDynamicEntity);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicDynamicEntity();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicDynamicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo KINEMATIC_MOVE. Estos mensajes  
		s�lo se utilizan para mover entidades de tipo cinem�tico.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el pr�ximo tick sobre la entidad cinem�tica. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact (IPhysics *otherComponent, bool enter);

	    //void onShapeHit(const physx::PxControllerShapeHit &hit);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) { onContact(otherComponent, true); }

		//Metodo que devuelve el booleano _inTrigger que nos indica si hay alguien dentro del trigger
		bool getInTrigger(){ return _inTrigger; };

		//Metodo que devuelve el booleano _inContact que nos indica si hay alguien tocando la entidad
		bool getInContact(){ return _inContact; };

		bool getInControllerContact(){ return _inControllerContact; };

		void setPosition(const Vector3 &position, bool makeConversionToLogicWorld);

		void setTransform(const Matrix4 &transform, bool makeConversionToLogicWorld);

		void addForce(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		/**
		Limpia las fuerzas acumuladas que podria llevar la entidad.
		*/
		void clearForce(Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void addTorque(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void deactivateSimulation();

		void activateSimulation();
		
		void move(const Vector3& disp);

	protected:

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Si la entidad f�sica es de tipo est�tico no hace nada. </li>
		<li> Si la entidad f�sica es de tipo din�mico actualiza la posici�n y rotaci�n de 
		     la entidad l�gica usando la informaci�n proporcionada por el motor de f�sica. </li>
		<li> Si la entidad f�sica es de tipo cinem�tico, adem�s solicita al motor de f�sica
		     que mueva la entidad de acuerdo al �ltimo mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onFixedTick(unsigned int msecs);

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		void createPhysicEntity(const Map::CEntity *entityInfo);
				
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de la informaci�n de mapa. 
		*/
		void createRigid(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);
		
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void createFromFile(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);

		// Servidor de f�sica
		Physics::CServer* _server;

		Physics::CGeometryFactory* _geometryFactory;

		Physics::CMaterialManager* _materialManager;

		// Actor que representa la entidad f�sica en PhysX
		//physx::PxRigidActor *_actor;

		Physics::CDynamicEntity _physicEntity;

		// Vector de deplazamiento recibido en el �ltimo mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades f�sicas cinem�ticas.
		Vector3 _movement;

		//Booleano que controla si hay alguien dentro del posible trigger que puede tener la entidad, por defecto desactivado
		bool _inTrigger;

		//Booleano que controla si hay alguien en contacto con la entidad, por defecto desactivado
		bool _inContact;

		bool _inControllerContact;

		bool _sleepUntil;

		bool _isTrigger;

		bool _noGravity;
	}; // class CPhysicDynamicEntity

	REG_FACTORY(CPhysicDynamicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
