#ifndef __Logic_WeaponsManager_H
#define __Logic_WeaponsManager_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic {
	class CShoot;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la activacion/desactivacion de las armas que tenemos.
*/


	class CWeaponsManager : public IComponent
	{
		DEC_FACTORY(CWeaponsManager);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CWeaponsManager();
		

	
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

		void changeWeapon(unsigned char newWeapon);
	
		void amplifyDamage(int percentage);

		void reduceCooldowns(int percentage);

		void addWeapon(int ammo, int weaponIndex);

	protected:
		/**
		Funci�n que se llama cuando se quiere cambiar de arma utilizando el scroll. A partir del arma actual,
		recorre el array del inventario de armas hacia adelante (iWeapon == 100) o hacia atr� (iWeapon == -100),
		en funci�n de la direcci�n que se le haya dado al scroll.
		@return Devuelve el �ndice del siguiente arma que tenemos (hacia arriba del inventario u hacia abajo)
		Si no ha encontrado ning�n arma siguiente, devuelve -1
		*/
		int selectScrollWeapon(int iWeapon);

		/**
		arma actual equipada
		*/
		int _currentWeapon;

		std::vector< std::pair<bool, CShoot*> > _weaponry;

	}; // class CShoot

	REG_FACTORY(CWeaponsManager);

} // namespace Logic

#endif // __Logic_Shoot_H
