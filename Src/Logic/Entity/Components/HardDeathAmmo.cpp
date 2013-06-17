/**
@file HardDeathAmmo.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CCoolDownAmmo
@see Logic::ISpellAmmo

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/



#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "HardDeathAmmo.h"
#include "HardDeath.h"
#include "HardDeathFeedback.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CHardDeathAmmo);

	//__________________________________________________________________

	CHardDeathAmmo::CHardDeathAmmo() : ISpellAmmo("hardDeath"){
		// Nada que hacer
	}

	//__________________________________________________________________

	CHardDeathAmmo::~CHardDeathAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CHardDeathAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		_friend[_friends] = _entity->getComponent<Logic::CHardDeath>("CHardDeath");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CHardDeathFeedback>("CHardDeathFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}
	//__________________________________________________________________



}//namespace Logic