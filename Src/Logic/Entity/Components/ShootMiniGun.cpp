/**
@file ShootMiniGun.cpp

Contiene la implementaci�n del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootMiniGun
@see Logic::IWeapon
*/

#include "ShootMiniGun.h"
#include "HudWeapons.h"

// Mapa
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageHudDispersion.h"

namespace Logic {
	IMP_FACTORY(CShootMiniGun);
	
	//__________________________________________________________________

	CShootMiniGun::~CShootMiniGun() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShootMiniGun::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IWeapon::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener los atributos obligatorios
		assert( entityInfo->hasAttribute(_weaponName + "Dispersion") );
		assert( entityInfo->hasAttribute(_weaponName + "Distance") );

		// Dispersi�n del arma
		_dispersionOriginal = _dispersion = entityInfo->getFloatAttribute(_weaponName + "Dispersion");
		// Distancia m�xima de disparo
		_distance = entityInfo->getFloatAttribute(_weaponName + "Distance");

		// Atributos opcionales de audio
		if( entityInfo->hasAttribute(_weaponName + "Audio") )
			_audioShoot = entityInfo->getStringAttribute(_weaponName + "Audio");

		return true;
	}

	//__________________________________________________________________

	void CShootMiniGun::process(const std::shared_ptr<CMessage>& message) {
		ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				
				if(type==Control::LEFT_CLICK) {
					_pressThenShoot=true;
					_bLeftClicked = true;
				}
				else if(type==Control::UNLEFT_CLICK) {
					_pressThenShoot=false;
					_bLeftClicked = false;
					_iContadorLeftClicked = 0;
					
					//Env�o el mensaje con valores para que resetee la mirilla
					auto m = std::make_shared<CMessageHudDispersion>();
					m->setTime(0);
					m->setReset(true);
					_entity->emitMessage(m);

					_bMensajeDispMandado = false;
					std::cout << "cambio mensajo dispmandado " << _bMensajeDispMandado << std::endl;
				}
				else if(type==Control::RIGHT_CLICK) {
					_acumulando = true;
				}
				else if(type==Control::UNRIGHT_CLICK) {
					_iRafagas = _contador / 10;
					_acumulando = false;
					_contador = 0;
				}
				
				break;
			}
		}
	} // process

	//__________________________________________________________________

	void CShootMiniGun::onFixedTick(unsigned int msecs) {
		// @deprecated Temporal hasta que este bien implementado
		CHudWeapons* hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		if(hudWeapon != NULL)
			hudWeapon->continouosShooting(_bLeftClicked);

		//std::cout << "fixed" << std::endl;
		if (_bLeftClicked) {
			++_iContadorLeftClicked;

			//std::cout << "Fixed = " << _iContadorLeftClicked << " y envio = " << _bMensajeDispMandado << std::endl;
			
			//Modificar la dispersi�n
			if ((_iContadorLeftClicked < 10) && (!_bMensajeDispMandado)) {
				_dispersion = _dispersionOriginal + 15.0f;
				//Enviamos el mensaje para que empiece a modificar la mirilla con la dispersi�n
				std::shared_ptr<CMessageHudDispersion> m = std::make_shared<CMessageHudDispersion>();
				m->setHeight(8.0f);
				m->setWidth(9.0f);
				m->setTime(2500);//Tiempo m�ximo que bajar� el tama�o de la mirilla
				m->setReset(false);
				_entity->emitMessage(m);
				_bMensajeDispMandado = true;
				printf("\nReduciendo mira");
				/**
				NOTA: De momento tiene el bug de que si disparas cuando no tienes munici�n, sigue haciendo la dispersi�n.
				La movida es que se sabe si tienes munici�n o no en el m�todo primaryShoot, de su padre ShootRaycast.
				Habr�a que hacer que este m�todo fuese un booleano y devolviese true si ha podido disparar y false si no.
				De momento no lo cambio porque creo que se va a cambiar la forma de gestionar estos raycast m�s adelante,
				pero hay que tenerlo en cuenta (tambi�n se tiene que tener en cuenta para cuando se ponga la animaci�n
				de vibraci�n de la minigun).
				*/
			}			
			else if (_iContadorLeftClicked < 20) {
				_dispersion = _dispersionOriginal + 5.0f;
			}
			else {
				_dispersion = _dispersionOriginal;
			}
		}


		if(_primaryCooldownTimer < _primaryCooldown) {
			_primaryCooldownTimer += msecs;
		}
		else {
			if(_pressThenShoot) {
				_primaryCanShoot=true;				
				primaryShoot();
			}
		}

		//Comprobamos la funcionalidad del bot�n derecho
		if (_acumulando) {
			//Si tenemos el bot�n derecho pulsado, seguimos aumentando el contador
			_contador++;
		}
		else {
			//No tenemos pulsado el derecho, as� que comprobamos si tenemos rafagas que lanzar
			if (_iRafagas > 0) {
				//Controlo que no se tengan m�s r�fagas del m�ximo (en su caso lo seteo a este valor)
				if (_iRafagas > _iMaxRafagas) {
					_iRafagas = _iMaxRafagas;
				}

				secondaryShoot(_iRafagas);
				_iRafagas = 0;

			}
		}
	}

} // namespace Logic

