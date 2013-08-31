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
#include "Physics/Aggregate.h"
#include "Graphics/Bone.h"

#include <vector>

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	// Predeclaraci�n de clase
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

		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void process(const std::shared_ptr<CMessage>& message);


		virtual std::string getPhysicName();


		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact(IPhysics *otherComponent, const Physics::CContactPoint& contactPoint, bool enter);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal);

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

		virtual void onActivate();

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void loadRagdoll(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);


		bool _ragdollHasControl;

		std::vector< std::pair< Graphics::CBone, Physics::CDynamicEntity* > > _ragdollBonesBuffer;

		Physics::CAggregate _aggregate;

	}; // class CRagdoll

	REG_FACTORY(CRagdoll);

} // namespace Logic

#endif // __Logic_Ragdoll_H
