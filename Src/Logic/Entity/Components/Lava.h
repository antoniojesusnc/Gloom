#ifndef __Logic_Jumper_H
#define __Logic_Jumper_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la aplicaci�n de la fuerza de salto a la entidad que ha tocado la entidad jumper.

*/
	class CLava : public IComponent
	{
		DEC_FACTORY(CLava);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CLava() : IComponent(){}
		

	
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
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
	
		/**
		Aplica una fuerza a la entidad
		*/
		//void applyJump(CEntity *entity);

	protected:

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Atributo que lleva la potencia del jumper
		*/
		float _damage;
		std::list<CEntity*> _intrigger;
		unsigned int _timestamp;
		unsigned int _timeDamage;

	}; // class CJumper

	REG_FACTORY(CLava);

} // namespace Logic

#endif // __Logic_Jumper_H
