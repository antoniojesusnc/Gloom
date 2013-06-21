//---------------------------------------------------------------------------
// AmplifyDamage.h
//---------------------------------------------------------------------------

/**
@file AmplifyDamage.h

Contiene la declaraci�n del componente que implementa el hechizo de amplificar da�o.

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_AmplifyDamage_H
#define __Logic_AmplifyDamage_H

#include "Logic/Entity/Components/Spell.h"


namespace Logic {
	class IWeapon;
}

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del gravity. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al gravity.

	@author Jose Antonio Garc�a Y��ez
	@date Febrero, 2013
	*/

	class CAmplifyDamage : public ISpell {
		DEC_FACTORY(CAmplifyDamage);

	public:

		/** Constructor por defecto. */
		CAmplifyDamage() : ISpell("amplifyDamage"), _newPercentDamage(0) { }

		//__________________________________________________________________

		virtual ~CAmplifyDamage();

		//__________________________________________________________________

		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>spellGravityDamageReflect:</strong> Da�o que hace el gravity al golpear contra el mundo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopSpell();

	private:
		

		int _newPercentDamage;

		std::vector< IWeapon*> _weaponry;
	}; // class CAmplifyDamage

	REG_FACTORY(CAmplifyDamage);

} // namespace Logic

#endif // __Logic__H
