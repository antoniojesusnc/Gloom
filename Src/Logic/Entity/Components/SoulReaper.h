//---------------------------------------------------------------------------
// SoulReaper.h
//---------------------------------------------------------------------------

/**
@file SoulReaper.h

Contiene la declaraci�n del componente que implementa el soulReaper.

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/

#ifndef __Logic_SoulReaper_H
#define __Logic_SoulReaper_H

#include "Logic/Entity/Components/Weapon.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del soulReaper. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al soulReaper.

	@author Jose Antonio Garc�a Y��ez
	@date Febrero, 2013
	*/

	class CSoulReaper : public IWeapon {
		DEC_FACTORY(CSoulReaper);

	public:

		/** Constructor por defecto. */
		CSoulReaper() : IWeapon("soulReaper"), _elementPulling(0), 
													_elementPulled(0),
													_shotsDistanceSecondaryFire(0), 
													_primaryFireTimer(0), 
													_primaryFireIsActive(false),
													_secondaryFireIsActive(false) { }

		//__________________________________________________________________

		virtual ~CSoulReaper();

		//__________________________________________________________________

		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>weaponSoulReaperDamageReflect:</strong> Da�o que hace el soulReaper al golpear contra el mundo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		void resetEntityPulling();

		float getHeight(){return _heightShoot;}

		
		virtual void amplifyDamage(unsigned int percentage);

		virtual void reduceCooldown(unsigned int percentage);

	private:

		void onTick(unsigned int msecs);
		/**
		Los disparos secundarios si fieren bastante unos de otros, por ahora se ha hecho un metodo vacio,
		para poder compilar por ahora, pero todos tendran que ser redefinidos.
		*/
		virtual void primaryFire();

		/**
		Los disparos secundarios si fieren bastante unos de otros, por ahora se ha hecho un metodo vacio,
		para poder compilar por ahora, pero todos tendran que ser redefinidos.
		*/
		virtual void secondaryFire();
		
		/**
		Este es el m�todo que todas las armas deben redefinir si quieren una accion cuando se suelta el boton de disparo secundario.
		Si no se redefine, no hara nada.
		*/
		virtual void stopSecondaryFire();

		/** M�todo est�tico que resetea la cantidad de munici�n del arma.
		En el soulReaper, se establecera a una bala, para que pueda disparar y debido a que cuando dispara no baja tendra balas infinitas
		*/
		virtual void resetAmmo();

		CEntity * checkPullItem();

		float _reboundForce;

		CEntity* _elementPulled, *_elementPulling;

		/* Indica la distancia de atraccion de objetos */
		float _shotsDistanceSecondaryFire;

		int _shotsDistance;

		float _defaultPrimaryFireDamage;
		float _primaryFireDamage;

		int  _defaultPrimaryFireCooldown;
		int _primaryFireCooldown;

		int _primaryFireTimer;

		bool _primaryFireIsActive;
		bool _secondaryFireIsActive;

	}; // class CShootRaycast

	REG_FACTORY(CSoulReaper);

} // namespace Logic

#endif // __Logic__H
