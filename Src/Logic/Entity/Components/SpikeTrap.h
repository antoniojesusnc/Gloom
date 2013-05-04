/**
@file SpikeTrap.h

Contiene la declaraci�n del componente que controla la aparicion de pinchos.

@see Logic::CSpikeTrap
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Mayo, 2013
*/
#ifndef __Logic_SpikeTrap_H
#define __Logic_SpikeTrap_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Entity.h"


//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la aparicion de los pinchos en el mapa.
	<p>
	Cuando la trampa sea tocada, los pinchos asociados a ella saldran disparados.
	
    @ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez
	@date Mayo, 2013
*/
	class CSpikeTrap : public IComponent
	{
		DEC_FACTORY(CSpikeTrap);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpikeTrap() : IComponent() {}

		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void onActivate();

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
		M�todo llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);


	private:

		/**
		Timer que lleva el control del cooldown.
		*/
		int _timer;

		/**
		CoolDown de activaci�n de la trampa de pinchos.
		*/
		int _coolDownTime;	

		/**
		Numero de la trampa.
		*/
		int _numTrap;

		/**
		Numero de pinchos asociados a la trampa.
		*/
		int _numSpikes;

		/**
		Velocidad de los pinchos.
		*/
		int _velocitySpikes;

		/**
		Booleano que controla que la trampa ya ha sido activada y esta en cd.
		*/
		bool _isRespawning;

		/**
		Direccion de los pinchos.
		*/
		Vector3 _directionSpikes;

		/**
		Pinchos asociados a la trampa.
		*/
		CEntity** _spikes;

	}; // class CSpikeTrap

	REG_FACTORY(CSpikeTrap);

} // namespace Logic

#endif // __Logic_SpikeTrap_H
