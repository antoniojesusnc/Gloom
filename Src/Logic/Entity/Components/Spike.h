/**
@file Spike.h

Contiene la implementaci�n del componente que controla los contactos con el pincho.

@see Logic::CSpike
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Febrero,Mayo, 2013
*/
#ifndef __Logic_Spike_H
#define __Logic_Spike_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla el contacto de los pinchos con los objetos.
	<p>
	Los pinchos volaran hasta tocar cualquier cosa y se destruiran.
	
    @ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez
	@date Febrero,Mayo, 2013
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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo touched (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo TOUCHED aplicamos una fuerza de salto al jugador.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);
	
	private:

		/**
		Posicion inicial del pincho.
		*/
		Vector3 _initialPosition;

	}; // class CSpike

	REG_FACTORY(CSpike);

} // namespace Logic

#endif // __Logic_Spike_H
