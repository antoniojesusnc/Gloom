/**
@file Elevator.h

Contiene la declaraci�n del componente que controla el componente de los kill streak

@see Logic::CKillStreak
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Agosto, 2013
*/
#ifndef __Logic_KillStreak_H
#define __Logic_KillStreak_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la gesti�n de los kill streak.
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Agosto, 2013
*/
	class CKillStreak : public IComponent
	{
		DEC_FACTORY(CKillStreak);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CKillStreak() : IComponent() {}

		
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
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Son siempre constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		void dead(const std::shared_ptr<CMessage>& message);

	private:

		int _iContKills;		

	}; // class CElevator

	REG_FACTORY(CKillStreak);

} // namespace Logic

#endif // __Logic_Elevator_H
