//---------------------------------------------------------------------------
// CharacterSound.h
//---------------------------------------------------------------------------

/**
@file CharacterSound.h


@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_CharacterSound_H
#define __Logic_CharacterSound_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"
#include "AvatarController.h"
#include "Physics.h"

#include <string>

namespace Audio {
	class CServer;
}

namespace Logic {

	/**


	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CCharacterSound : public IComponent, public CAvatarController::IObserver,
							public IPhysics::IObserver {
		DEC_FACTORY(CCharacterSound);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CCharacterSound();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CCharacterSound();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>primarySkillCooldown:</strong> Tiempo de recarga de la habilidad primaria. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);


		// =======================================================================
		//             METODOS HEREDADOS DE CAVATARCONTROLLER::IOBSERVER
		// =======================================================================


		virtual void onLand();
		virtual void onWalk();
		virtual void onJump();
		virtual void onDodge();
		virtual void onIdle();
		virtual void onAir();


		// =======================================================================
		//				 METODOS HEREDADOS DE IPHYSICS::IOBSERVER
		// =======================================================================

		virtual void onShapeHit(IPhysics* otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal);

	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================

		virtual void onStart();

		virtual void onTick(unsigned int msecs);

		virtual void onActivate();

		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		bool _playerIsWalking;

		int _footstepTimer;

		unsigned int _footstepTimeStep;

		Audio::CServer* _audioServer;

		CAvatarController* _avatarController;

	}; // class CCharacterSound

	REG_FACTORY(CCharacterSound);

} // namespace Logic

#endif // __Logic_Footsteps_H
