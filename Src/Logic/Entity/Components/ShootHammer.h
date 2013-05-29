//---------------------------------------------------------------------------
// ShootHammer.h
//---------------------------------------------------------------------------

/**
@file ShootHammer.h

Contiene la declaraci�n del componente que implementa el hammer.

@author Jose Antonio Garc�a Y��ez
@date Febrero, 2013
*/

#ifndef __Logic_ShootHammer_H
#define __Logic_ShootHammer_H

#include "Logic/Entity/Components/Weapon.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del hammer. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al hammer.

	@author Jose Antonio Garc�a Y��ez
	@date Febrero, 2013
	*/

	class CShootHammer : public IWeapon {
		DEC_FACTORY(CShootHammer);

	public:

		/** Constructor por defecto. */
		CShootHammer() : IWeapon("hammer"), _elementPulling(0), _elementPulled(0), _shotsDistanceSecondaryFire(0) { }

		//__________________________________________________________________

		virtual ~CShootHammer();

		//__________________________________________________________________

		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>weaponHammerDamageReflect:</strong> Da�o que hace el hammer al golpear contra el mundo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		void resetEntityPulling();

		float getHeight(){return _heightShoot;}

	protected:

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
		virtual void stopSecondaryFire(unsigned int elapsedTime);

		/** M�todo est�tico que resetea la cantidad de munici�n del arma.
		En el hammer, se establecera a una bala, para que pueda disparar y debido a que cuando dispara no baja tendra balas infinitas
		*/
		virtual void resetAmmo();

		CEntity * fireSecondary();

		float _reboundForce;

		CEntity* _elementPulled, *_elementPulling;

		/* Indica la distancia de atraccion de objetos */
		float _shotsDistanceSecondaryFire;

	}; // class CShootRaycast

	REG_FACTORY(CShootHammer);

} // namespace Logic

#endif // __Logic_Shoot_H
