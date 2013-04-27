/**
@file Hacksaw.h

Contiene la declaraci�n del componente que controla el movimiento de la sierra mec�nica

@see Logic::CHacksaw
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/
#ifndef __Logic_Hacksaw_H
#define __Logic_Hacksaw_H

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
	class CHacksaw : public IComponent
	{
		DEC_FACTORY(CHacksaw);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHacksaw() : IComponent() {}

		
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
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);
		
		/**
		Posici� inicial y final del ascensor.
		*/
		Vector3 _positionInitial;
		Vector3 _positionFinal;

		/**
		Controla si vamos hacia la posicion final o no
		*/
		bool _toFinal;
	
		/**
		Direcci�n inicial y final del ascensor.
		*/
		Vector3 _directionFinal;
		Vector3 _directionInitial;

		/**
		Velocidad del ascensor
		*/
		float _velocity;

	}; // class CHacksaw

	REG_FACTORY(CHacksaw);

} // namespace Logic

#endif // __Logic_Hacksaw_H
