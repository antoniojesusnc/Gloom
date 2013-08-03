/**
@file AnimationManager.h

Contiene la declaraci�n del componente que controla las animaciones
que posee esta entidad en cualquier ordenador

@see Logic::AnimationManager

@author Rub�n Mulero
@date June, 2013
*/

#include "Logic/Entity/Component.h"

#ifndef __Logic_AnimationManager_H
#define __Logic_AnimationManager_H
//declaraci�n de la clase
namespace Logic 
{
	//forward declarations
	class CAvatarController;
/**
	Componente que se encarga de las animaciones que tiene que poner la entidad
	en caso de que posea el componente adecuado para poner animaciones
	<p>
	Se encarga de, seg�n el movimiento del personaje y sus acciones, enviar 
	mensajes de SET_ANIMATION y STOP_ANIMATION por la red para que los otros
	clientes que tengan esta entidad pongan las animaciones adecuadas
	
    @ingroup logicGroup
*/
	class CAnimationManager: public IComponent
	{
		DEC_FACTORY(CAnimationManager);
	public:

		struct Animation{
			std::string animation;
			int rewind;
		};



		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimationManager();

		/**
		destructor
		*/
		virtual ~CAnimationManager() {}

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity).

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>defaultAnimation</strong> Animaci�n por defecto que pone la entidad al spawnear en el mapa </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de inercia y desplazamiento.
		*/
		//virtual void onActivate();

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>PLAYER_DEAD</li
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

	protected:

		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================


		/**
		Setea el puntero al componente del controlador f�sico para evitar overhead
		de mensajes en el tick.

		@param unsigned int msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onStart();

		//________________________________________________________________________

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Son siempre
		constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================

		void sendDeadMessage(TEntityID killer);

		std::string getMotionAnimation(const Vector3 &displacementDir);


		void initAnimationCommands();


	private:

		// =======================================================================
		//                           MIEMBROS DE CLASE
		// =======================================================================
		/**
		Tabla que representa las animaciones que el personaje debe poner
		cuando se est� moviendo por el suelo
		*/

		Vector3 _lastDisplacementAnimation;

		CAvatarController* _avatarController;

		bool _flying;

		typedef std::pair<std::string, Animation> TAnimation;
		std::map<std::string, Animation> _animations;
	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimationManager);

} // namespace Logic
#endif