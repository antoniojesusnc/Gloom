/**
@file CameraFeedbackNotifier.h

Contiene la declaraci�n del componente que 
controla los da�os recibidos por el escudo
del Screamer.

@see Logic::CCameraFeedbackNotifier
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Abril, 2013
*/

#ifndef __Logic_CameraFeedbackNotifier_H
#define __Logic_CameraFeedbackNotifier_H

#include "Logic/Entity/Component.h"


namespace Graphics{
	class CScene;
}

namespace Logic {
	class CCamera;
	class CAvatarController;
}

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Abril, 2013
	*/
	
	class CCameraFeedbackNotifier : public IComponent {
		DEC_FACTORY(CCameraFeedbackNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CCameraFeedbackNotifier();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CCameraFeedbackNotifier();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		virtual bool spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo);

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>SET_REDUCED_DAMAGE</li>
		</ul>
		
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Manda un mensaje a la c�mara para que vibre
		*/
		void damaged();

		//__________________________________________________________________

		/**
		Este m�todo debe ser usado por aquella entidad que cree a esta entidad,
		para indicar que �sta �ltima le pertenece.

		@param owner Entidad l�gica que gener� a esta entidad.
		*/
		void setOwner(Logic::CEntity* owner);

		void playerIsWalking(bool walking) { _playerIsWalking = walking; }

		void walkEffect(unsigned int msecs);


		void playerIsTouchingGround(float hitForce);

		void landEffect(unsigned int msecs);


		/*
		M�todo que calcula desde donde nos han da�ado, para indicarlo como feedback en el 
		circulito de la mirilla en funci�n de dicha posici�n
		*/
		void calculateEnemyPosition();

	protected:

		virtual void onStart();

		virtual void onFixedTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Entidad due�a de esta entidad. */
		CEntity* _owner;

		CCamera* _cameraComponent;

		float _currentWalkingRoll;

		float _walkingRollSpeed;

		float _walkingRollOffset;

		bool _playerIsWalking;

		bool _playerIsLanding;

		float _landForce;

		float _landRecoverySpeed;

		float _currentLandOffset;

		float _maxVelocity;
		Logic::CAvatarController* _avatarc;

		bool _effectIsActivated;
		std::string _effect, _motionblur;
		std::string _strengthEffect;
		Graphics::CScene *_scene;
		int _timestamp;


	}; // class CScreamerShieldDamageNotifier

	REG_FACTORY(CCameraFeedbackNotifier);

} // namespace Logic

#endif // __Logic_CameraFeedbackNotifier_H