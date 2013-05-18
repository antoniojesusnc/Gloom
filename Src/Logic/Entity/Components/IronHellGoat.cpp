/**
@file IronHellGoat.cpp

Contiene la implementacion del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::CShootProjectile

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#include "IronHellGoat.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

namespace Logic {
	
	IMP_FACTORY(CIronHellGoat);

	//__________________________________________________________________

	CIronHellGoat::CIronHellGoat() : CShootProjectile("ironHellGoat"),
									 _primaryFireIsActive(false),
									 _secondaryFireIsActive(false) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CIronHellGoat::~CIronHellGoat() {

	}

	//__________________________________________________________________

	bool CIronHellGoat::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !CShootProjectile::spawn(entity, map, entityInfo) ) return false;

		return true;
	}

	//__________________________________________________________________

	void CIronHellGoat::onTick(unsigned int msecs) {
		// CShoot::onTick(msecs);

		// Si el jugador esta dejando pulsado el disparo primario, aumentamos
		// el tama�o de la bola y reducimos la velocidad hasta un limite
		if(_primaryFireIsActive) {
			// Si la bola no tiene el tama�o m�ximo, aumentarla conforme
			// al factor de crecimiento establecido y reducir su velocidad

		}
		else if(_secondaryFireIsActive) {
			// Si hay bolas vivas, mover las en funci�n de la velocidad que
			// llevan y la direcci�n en la que apunto con el rat�n

			// Comprobar el buffer de bolas vivas
		}
		else {
			// Si no quedan bolas vivas poner a dormir
		}
	}

	//__________________________________________________________________

	void CIronHellGoat::primaryShoot() {
		std::cout << "Primario" << std::endl;
		// Si tenemos suficiente munici�n
			_primaryFireIsActive = true;
		// Si no tenemos suficiente munici�n ponemos el sonido de sin balas
	}

	//__________________________________________________________________

	void CIronHellGoat::stopPrimaryShoot() {
		_primaryFireIsActive = false;

		// Crear la bola de fuego con los par�metros establecidos
		// Para eso obtengo la plantilla del mapa y la modifico. Luego
		// le pido a la factoria que me cree una entidad con esa plantilla.
		// Seteamos un owner a la bola para que luego nos pueda llamar
		// al destruirse para que la desapuntemos.

		// Me apunto la entidad devuelta por la factoria

		// Reseteo los valores de creaci�n
	}

	//__________________________________________________________________

	void CIronHellGoat::secondaryShoot() {
		std::cout << "Secundario" << std::endl;
		_secondaryFireIsActive = true;
	}

	//__________________________________________________________________

	void CIronHellGoat::stopSecondaryShoot() {
		_secondaryFireIsActive = false;
	}

	//__________________________________________________________________

	void CIronHellGoat::forgetFireBall(Logic::CEntity* fireBall) {
		// Borrar esta bola de la lista de bolas que podemos
		// controlar.
	}

}//namespace Logic