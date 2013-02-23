/**
@file AvatarController.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes m�todos que permiten avances o giros. El uso de este componente
	es un poco at�pico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las �rdenes que se deben
	llevar a cabo mediante llamadas a m�todos p�blicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las �rdenes, sin embargo de momento as� es suficiente.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	class CAvatarController : public IComponent
	{
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(), _jumping(false), _walking(false), _walkingBack(false), 
			_strafingLeft(false), _strafingRight(false), _speed(0.05f) {}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual void activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra as� mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n.
		*/
		virtual void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		/**
		Provoca que la entidad avance.
		*/
		void walk();

		/**
		Provoca que la entidad retroceda.
		*/
		void walkBack();

		/**
		Provoca que la entidad cese el avance.
		*/
		void stopWalk();

		/**
		Provoca que la entidad cese el retroceso
		*/
		void stopWalkBack();

		/**
		Provoca que la entidad se desplace lateralmente a la izquierda.
		*/
		void strafeLeft();

		/**
		Provoca que la entidad se desplace lateralmente a la derecha.
		*/
		void strafeRight();

		/**
		Provoca que la entidad cese el desplazamiento lateral derecho.
		*/
		void stopStrafeRight();
		
		/**
		Provoca que la entidad cese el desplazamiento lateral izquierdo.
		*/
		void stopStrafeLeft();

		/**
		Provoca que la entidad salte.
		*/
		void jump();

		/**
		Provoca que la entidad rebote
		*/
		void rebound();

		/**
		Provoca que a la entidad se le aplique una fuerza
		*/
		void force();

		/**
		Provoca que la entidad gire. N�meros Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void mouse(const float* amount);

	protected:

		/**
		Atributo para saber si la entidad est� avanzando.
		*/
		bool _walking;

		/**
		Atributo para saber si la entidad est� retrocediendo.
		*/
		bool _walkingBack;

		/**
		Atributo para saber si la entidad est� moviendose lateralmente
		a la izquierda.
		*/
		bool _strafingLeft;

		/**
		Atributo para saber si la entidad est� moviendose lateralmente
		a la derecha.
		*/
		bool _strafingRight;

		/**
		Atributos para el control del salto de la entidad
		*/
		bool _jumping;
		bool _jumpingControl;
		bool _canJump;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;

		/**
		Atributo que indica la magnitud de velocidad del salto de la entidad.
		*/
		double _speedJump;

		/**
		Atributo que indica la magnitud de la gravedad para el MRUA
		*/
		double _gravity;

		/**
		Atributo que indica si estamos cayendo (se actualizar� mediante un mensaje que nos envia el PhysicController)
		*/
		bool _falling;

		/**
		Atributo que indica la direcci�n en el momento de saltar o de empezar a caer
		*/
		Vector3 _direccionSaltoCaida;

		/**
		Atributo que indica si la entidad empez� a caer
		*/
		bool _caida;

		/**
		Atributos que se encargan de contar/llevar el tiempo para ver si se hizo un salto lateral
		*/
		int _jumpLeft;
		int _jumpRight;
		int _timeSideJump;
		bool _sideJump;
		bool _velocitySideJump;

		bool _unpressRight;
		bool _unpressLeft;
		bool _readySideJumpLeft;
		bool _readySideJumpRight;
		bool _dontCountUntilUnpress;

		/**
		Atributo que lleva el conteo de saltos concatenados para el aumento de velocidad
		*/
		int _nConcatSideJump;
		int _timeConcatSideJump;
		bool _activeConcat;
		bool _sideFly;
		bool _sideContact;

		/**
		Atributo que lleva la direcci�n en la que tiene que ir el rebote del jugador
		*/
		Vector3 _dirRebound;

		/**
		Atributo que dice si el rebote est� activo
		*/
		bool _rebound;

		/**
		Atributo que dice si hay que aplicar una fuerza
		*/
		bool _force;

		/**
		Atributo que indica la potencia explosiva que se el aplicar� al salto lateral al iniciarse
		*/
		float _powerSideJump;

		/**
		Atributo que indica la potencia explosiva que se el aplicar� al salto al iniciarse
		*/
		float _powerJump;

		/**
		Atributo que indica la potencia explosiva que se el aplicar� al salto en jumper al iniciarse (eje Y)
		*/
		float _powerJumpForce;

		/**
		Atributo que indica la potencia explosiva que se el aplicar� al salto en jumper al iniciarse (eje X y Z)
		*/
		float _velocityForce;

		/**
		Atributo que indica la potencia explosiva que se el aplicar� al salto en jumper al iniciarse (eje X y Z)
		*/
		Vector3 _directionForce;

		/**
		Atributo que indica que hay que aplicar la velocidad de jumper
		*/
		bool _applyForce;


		/**
		Atributos que indican que velocidad extra tendr� el salto lateral, segun si no es concatenado o s� respectivamente
		*/
		float _sumSpeedSideJump;
		float _sumSpeedSideJumpConcat;

		/**
		Atributo que indica el factor con el que el movimiento actual afectar� a la direcci�n del jugador cuando se encuentra en el aire
		*/
		float _restitutionMoveAir;

		/**
		Atributos que indican el maximo tiempo para contabilizar un salto lateral y la concatenacion de un salto lateral respectivamente
		*/
		int _maxTimeSideJump;
		int _maxTimeConcatSideJump;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
