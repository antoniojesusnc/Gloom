/**
@file Spell.cpp

Contiene la implementaci�n de la interfaz com�n
a todas las armas.

@see Logic::ISpell
@see Logic::IComponent

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#include "Spell.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Messages/MessagePrimarySpell.h"
#include "Logic/Messages/MessageSecondarySpell.h"

using namespace std;

namespace Logic {
	
	ISpell::ISpell(const string& spellName) : _spellName("spell" + spellName)
												{

		// Nada que inicializar
	}

	//__________________________________________________________________

	ISpell::~ISpell() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpell::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_spellName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_spellName + "ID") );
		assert( entityInfo->hasAttribute("physic_radius") );
		assert( entityInfo->hasAttribute("heightShoot") );

		// Leemos los atributos obligatorios de arma
		_spellID = (SpellType::Enum)entityInfo->getIntAttribute(_spellName + "ID");
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");

		return true;
	}

	//__________________________________________________________________

	bool ISpell::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PRIMARY_SPELL ||
			   msgType == Message::SECONDARY_SPELL;
	}

	//__________________________________________________________________

	void ISpell::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SPELL: {
				shared_ptr<CMessagePrimarySpell> primarySpellMsg = static_pointer_cast<CMessagePrimarySpell>(message);
				if(!_isPassive && _isPrimarySpell){
					if( primarySpellMsg->getSpell() )
						spell();
					else
						stopSpell();
				}
				break;
			}
			case Message::SECONDARY_SPELL: {
				shared_ptr<CMessageSecondarySpell> secondarySpellMsg = static_pointer_cast<CMessageSecondarySpell>(message);
				if(!_isPassive && !_isPrimarySpell){
					if( secondarySpellMsg->getSpell() )
						spell();
					else
						stopSpell();
				}
				break;
			}
		}
	}

	//__________________________________________________________________

	void ISpell::onAvailable() {

	}
	//_________________________________________________________________

} // namespace Logic

