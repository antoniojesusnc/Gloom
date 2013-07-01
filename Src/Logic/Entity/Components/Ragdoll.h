/**
@file Ragdoll.h


@see Logic::CRagdoll
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Logic_Ragdoll_H
#define __Logic_Ragdoll_H

#include "Physics.h"
#include "Physics/DynamicEntity.h"

#include <vector>

// Predeclaraci�n de tipos
namespace Physics {
	class CServer;
	class CGeometryFactory;
	class CMaterialManager;
};

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	class CAnimatedGraphics;

	/**
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CRagdoll : public IPhysics {
		DEC_FACTORY(CRagdoll);
	public:
	
		/**
		Constructor por defecto.
		*/
		CRagdoll();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CRagdoll();
		
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

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) { onContact(otherComponent, true); }

		void addForce(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void addTorque(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void deactivateSimulation();

		void activateSimulation();
		
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

		virtual void onStart();

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void loadRagdoll(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);

		CAnimatedGraphics* _animatedGraphicsComponent;

		// Servidor de f�sica
		Physics::CServer* _server;

		Physics::CGeometryFactory* _geometryFactory;

		Physics::CMaterialManager* _materialManager;

		Physics::CDynamicEntity _physicEntity;

	}; // class CRagdoll

	REG_FACTORY(CRagdoll);

} // namespace Logic

#endif // __Logic_Ragdoll_H
