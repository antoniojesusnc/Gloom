/**
@file PhysicStaticCompound.h

@see Logic::CPhysicStaticCompound

@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Logic_PhysicStaticEntity_H
#define __Logic_PhysicStaticEntity_H

#include "Physics.h"
#include <vector>

namespace Physics {
	class CStaticEntity;
}

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	/**

    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CPhysicStaticCompound : public IPhysics {
		DEC_FACTORY(CPhysicStaticCompound);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicStaticCompound();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicStaticCompound();
		
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

		
 
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact (IPhysics *otherComponent, bool enter);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal);



		void deactivateSimulation();

		void activateSimulation();

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
		void deserializeEntities(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);



		std::vector<Physics::CStaticEntity*> _physicEntitiesList;

	}; // class CPhysicStaticCompound

	REG_FACTORY(CPhysicStaticCompound);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
