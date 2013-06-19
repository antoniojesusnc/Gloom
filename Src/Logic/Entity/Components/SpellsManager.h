#ifndef __Logic_SpellsManager_H
#define __Logic_SpellsManager_H

#include "Logic/Entity/Component.h"
#include "SpellType.h"

#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic {
	class ISpellAmmo;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la activacion/desactivacion de las armas que tenemos.
*/


	class CSpellsManager : public IComponent
	{
		DEC_FACTORY(CSpellsManager);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpellsManager();
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo SHOOT (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo SHOOT la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

				/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void onDeactivate();

		void reduceCooldowns(int percentage);

		virtual void onStart();
	protected:

		std::vector< std::pair<bool, ISpellAmmo*> > _spells;

		SpellType::Enum _primarySpell;

		SpellType::Enum _secondarySpell;

	}; // class CShoot

	REG_FACTORY(CSpellsManager);

} // namespace Logic

#endif // __Logic_Shoot_H
