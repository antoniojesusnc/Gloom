/**
@file EnemyInSight.h

@see Logic::CEnemyInSight
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Agosto, 2013
*/

#ifndef __Logic_EnemyInSight_H
#define __Logic_EnemyInSight_H

#include "Logic/Entity/Component.h"

namespace Physics {
	class CServer;
}

namespace Graphics {
	class CMovable2dText;
}

namespace Logic {

	class CEntity;

	/**

    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Agosto, 2013
	*/

	class CEnemyInSight : public IComponent {
		DEC_FACTORY(CEnemyInSight);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CEnemyInSight();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CEnemyInSight();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>lightType:</strong> Tipo de luz, focal, puntual o direccional. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================



	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================
		

		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		std::map<CEntity*, int> _namesBeingShown;
		int _raycastTimer;
		unsigned int _raycastTimeStep;
		unsigned int _visibilityTimeStep;
		Physics::CServer* _physicsServer;
		float _heightShoot;

	}; // class CEnemyInSight

	REG_FACTORY(CEnemyInSight);

} // namespace Logic

#endif // __Logic_CharacterName_H
