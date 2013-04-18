/**
@file Spike.h

Contiene la declaraci�n del componente que controla la aparicion de pinchos.

@see Logic::CSpike
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/
#ifndef __Logic_Spike_H
#define __Logic_Spike_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la aparicion de los pinchos en el mapa.
	<p>
	Los pinchos aparecer�n cada cierto tiempo.
	
    @ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez
	@date Febrero, 2013
*/
	class CSpike : public IComponent
	{
		DEC_FACTORY(CSpike);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpike() : IComponent() {}

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void activate();

		/**
		Este componente s�lo acepta mensajes de tipo touched (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo TOUCHED aplicamos una fuerza de salto al jugador.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);
	

	protected:

		/**
		Timers que controlan la aparici�n de los pinchos.
		*/
		unsigned int _timer;
		unsigned int _timeToShow;
		unsigned int _timeToUnshow;
		
		/**
		Booleano que controla si esta activo.
		*/
		bool _active;

			/**
		Posici� inicial y final del pincho.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;

		/**
		Controla si vamos hacia la posicion final o no.
		*/
		bool _toFinal;
	
		/**
		Direcci�n inicial y final del pincho.
		*/
		Vector3 _directionFinal;
		Vector3 _directionInitial;

		/**
		Velocidad del pincho
		*/
		float _velocity;

		/**
		Booleano que controla la espera para no ejecutar instrucciones inutiles.
		*/
		bool _wait;

	}; // class CSpike

	REG_FACTORY(CSpike);

} // namespace Logic

#endif // __Logic_Spike_H
