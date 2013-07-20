/**
@file ProceduralGraphics.h

Contiene la implementaci�n del componente que controla las animaciones
procedurales que se aplican a la entidad
 
@see Logic::ProceduralGraphics
@see Logic::IComponent

@author Rub�n Mulero Guerrero
@date June, 2013
*/
#ifndef __Logic_ProceduralGraphics_H
#define __Logic_ProceduralGraphics_H

#include "Logic/Entity/Component.h"
#include "Graphics/AnimatedEntity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CAnimatedEntity;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica animada de una 
	entidad. Este componente no se encarga de cargar animaciones ni de 
	quitarlas, sino que se encarga del movimiento de huesos de manera 
	procedural para, por ejemplo, hacer que la cabeza y los brazos del
	personaje apunten en la direcci�n en la que �ste est� apuntando
	
    @ingroup logicGroup

	@author Rub�n Mulero Guerrero
	@date June, 2013
*/
	class CProceduralGraphics : public IComponent, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CProceduralGraphics);
	public:
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CProceduralGraphics();

		//________________________________________________________________________

		/** Destructor por defecto. */
		virtual ~CProceduralGraphics();


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
			<li><strong>bone:</strong> El nombre del hueso que estamos animando. </li>
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
		virtual void onActivate();

		/**
		M�todo que desactiva el componente.

		@return true si todo ha ido correctamente.
		*/
		virtual void onDeactivate();

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
			<li>ADDFORCEPLAYER</li>
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
		//             M�TODOS HEREDADOS DE CANIMATEDENTITYLISTENER
		// =======================================================================
		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::string &animation);

	protected:
		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Son siempre
		constantes.
		*/
		virtual void onTick(unsigned int msecs);

	private:

		/**
		Entidad gr�fica animada.
		*/
		Graphics::CAnimatedEntity *_animatedEntity;

		/** Nombre del hueso que vamos a mover. */
		std::string _masterBoneName;

		/** Cota superior de pitch. */
		float _upperPitchBound;

		/** Cota inferior de pitch. */
		float _lowerPitchBound;

		/** Cota superior de yaw. */
		float _upperYawBound;

		/** Cota inferior de yaw. */
		float _lowerYawBound;

		bool _insertAnimation;

	}; // class CAnimatedGraphics

	REG_FACTORY(CProceduralGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H