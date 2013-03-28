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

#include "Logic/Entity/Components/ShootRaycast.h"

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

	class CShootHammer : public CShootRaycast {
		DEC_FACTORY(CShootHammer);

	public:

		/** Constructor por defecto. */
		CShootHammer() : CShootRaycast("Hammer") { }

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

	protected:

		/**
		Redefine el m�todo de decremento de munici�n, que en este caso se deja vacio, 
		ya que el hammer no tiene munici�n.
		*/
		virtual void decrementAmmo();

		//__________________________________________________________________

		/**
		M�todo que se encarga de mandar los mensajes que corresopondan a la entidad
		que se ha golpeado en caso de hacer hit.

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit);


	}; // class CShootRaycast

	REG_FACTORY(CShootHammer);

} // namespace Logic

#endif // __Logic_Shoot_H
