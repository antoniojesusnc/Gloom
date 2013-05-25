//---------------------------------------------------------------------------
// ShootSniper.h
//---------------------------------------------------------------------------

/**
@file ShootSniper.h

Contiene la declaraci�n del componente que implementa el arma sniper.

@author Antonio Jesus Narv�ez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_ShootSniper_H
#define __Logic_ShootSniper_H

#include "Logic/Entity/Components/ShootRaycast.h"
#include "Physics/RaycastHit.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementaci�n por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narv�ez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CShootSniper : public CShootRaycast {
		DEC_FACTORY(CShootSniper);

	public:

		/** Constructor por defecto. */
		CShootSniper() : CShootRaycast("sniper") {}

		virtual ~CShootSniper();

		//__________________________________________________________________

		
		/**
		Redefinimos porque la sniper tendr� un comportamiento diferente.
		*/
		virtual void primaryShoot();

		/**
		Redefinimos porque la sniper tendr� un comportamiento diferente.
		*/
		virtual void secondaryShoot();

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		
		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		M�todo que se encarga de realizar el disparo primario del raycast por fisicas.
		Tambi�n aplicar� el da�o.

		*/
		void primaryFireWeapon();


		/**
		M�todo que se encarga de realizar el disparo secundario del raycast por fisicas.
		Tambi�n aplicar� el da�o.
		*/
		void secondaryFireWeapon();


		/**
		M�todo que se encarga de intentar aplicar da�o por expansi�n al utilizar el disparo secundario de la sniper.

		@param entityHit Entidad a partir de la cual queremos expandir el da�o.
		@return Devuelve la entidad a la que hay que aplicar da�o de expansi�n.
		*/
		CEntity* findEnemyToExpand(CEntity* entityHit);

	private:

		/**
		Distancia m�xima de expansi�n para el disparo secundario de la sniper.
		*/
		float _maxExpansiveDistance;

		/**
		Numero de balas que consume el disparo secundario.
		*/
		int _secondaryConsumeAmmo;

	}; // class CShootSniper

	REG_FACTORY(CShootSniper);

} // namespace Logic

#endif // __Logic_Shoot_H
