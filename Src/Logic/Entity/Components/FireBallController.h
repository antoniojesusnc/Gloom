/**
@file FireBallController.h

Contiene la declaraci�n de la cabecera del componente
que controla las acciones de la bola de fuego.

@see Logic::CFireBallController
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_FireBallController_H
#define __Logic_FireBallController_H

#include "Logic/Entity/Component.h"
#include "Physics/ContactPoint.h"

namespace Logic {
	class CIronHellGoat;
	class CPhysicDynamicEntity;
	class CEntity;
}

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/
	
	class CFireBallController : public IComponent {
		DEC_FACTORY(CFireBallController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CFireBallController();

		virtual ~CFireBallController();

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
			<li><strong>: </strong>.</li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li></li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CIronHellGoat* _owner);

		// @todo Setear una nueva direccion calculada en base a la direccion donde
		// apunta el puntero

		void alterDirection(const Vector3& direction);

	protected:

		virtual void onStart();

		/**

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onFixedTick(unsigned int msecs);

	private:

		void createExplotion(const Physics::CContactPoint& contactPoint);

		void estimateDamage(CEntity* entityHit, const Vector3& explotionPos);

		CIronHellGoat* _owner;

		CPhysicDynamicEntity* _physicComponent;

		Vector3 _direction;

		float _speed;

		float _damage;

		float _explotionRadius;

		float _strength;

		Logic::CEntity *_world;
	}; // class CFireBallController

	REG_FACTORY(CFireBallController);

} // namespace Logic

#endif
