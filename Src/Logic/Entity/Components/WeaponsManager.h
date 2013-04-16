#ifndef __Logic_WeaponsManager_H
#define __Logic_WeaponsManager_H

#include "Logic/Entity/Component.h"

#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic {
	class CShoot;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la capacidad de disparo de una entidad, asi como las armas que puede tener. Procesa mensajes de tipo 
	SHOOT (indican que la entidad ha disparado)
	<p>
	Poseera una vector, con las armas que tiene, asi como su da�o y alcance.
	Tambien poseera un vector de booleanos que indicara las armas que posee.

*/


	class CWeaponsManager : public IComponent
	{
		DEC_FACTORY(CWeaponsManager);

	public:

		enum WeaponType {
			eHAMMER,
			eSNIPER,
			eSHOTGUN,
			eMINIGUN,
			eGRENADE_LAUNCHER,
			eROCKET_LAUNCHER
		};

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
		virtual void activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		void changeWeapon(unsigned char newWeapon);
	
		void amplifyDamage(int percentage);

		void reduceCooldowns(int percentage);

		void addWeapon(int ammo, int weaponIndex);

	protected:


		/**
		arma actual equipada
		*/
		int _currentWeapon;


		/**
		numero de armas que tengo
		*/
		const unsigned int _numWeapons;

		std::vector< std::pair<bool, CShoot*> > _weaponry;

	}; // class CShoot

	REG_FACTORY(CWeaponsManager);

} // namespace Logic

#endif // __Logic_Shoot_H
