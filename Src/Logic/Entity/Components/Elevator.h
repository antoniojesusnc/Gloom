/**
@file Elevator.h

Contiene la declaraci�n del componente que controla el movimiento de los ascensores

@see Logic::CElevator
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/
#ifndef __Logic_Elevator_H
#define __Logic_Elevator_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el da�o a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animaci�n de muerte?)
	y si es el jugador habr� que terminar el juego. Si la vida sigue siendo mayor 
	que 0 tr�s un golpe �poner la animaci�n de herido?.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Octubre, 2010
*/
	class CElevator : public IComponent
	{
		DEC_FACTORY(CElevator);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CElevator() : IComponent() {}

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
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		/**
		Nos movemos a la posici�n inicial
		*/
		void toInitial();
	
		/**
		Nos movemos a la posicion final
		*/
		void toFinal();

	protected:

		/**
		Posicion inicial y final del ascensor.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;


	}; // class CElevator

	REG_FACTORY(CElevator);

} // namespace Logic

#endif // __Logic_Elevator_H
