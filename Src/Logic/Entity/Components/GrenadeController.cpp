/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#include "GrenadeController.h"

#include "Physics/Server.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Logic {
	
	IMP_FACTORY(CGrenadeController);

	//________________________________________________________________________

	CGrenadeController::CGrenadeController() : _timer(0) { 
		// Nada que hacer
	}
	
	//________________________________________________________________________

	void CGrenadeController::tick(unsigned int msecs) {
		// Actualizamos el timer. Si se ha cumplido el tiempo limite de explosion
		// eliminamos la entidad granada y creamos la entidad explosion.
		_timer += msecs;
		if(_timer > _explotionTime) {
			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity,false);
			// Creamos la explosion
			createExplotion();
		}
	} // tick

	//________________________________________________________________________

	bool CGrenadeController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Leer el timer que controla la explosion
		if( entityInfo->hasAttribute("explotionTime") ) {
			// Pasamos a msecs
			_explotionTime = entityInfo->getFloatAttribute("explotionTime") * 1000;
		}

		_explotionDamage = entityInfo->getFloatAttribute("explotionDamage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");
		_explotionForce = entityInfo->getFloatAttribute("explotionForce");
		_audioExplotion = entityInfo->getStringAttribute("explotionAudio");
		_explotionActive = false;

		return true;
	} // spawn

	//________________________________________________________________________

	bool CGrenadeController::accept(const std::shared_ptr<CMessage>& message) {
		//Solamente podemos aceptar un contacto porque luego explotamos
		return message->getMessageType() == Message::CONTACT_ENTER && !_explotionActive ||
			message->getMessageType() == Message::CONTACT_EXIT && !_explotionActive;
	} // accept
	
	//________________________________________________________________________

	void CGrenadeController::process(const std::shared_ptr<CMessage>& message) {
		if(!_explotionActive) {
			switch( message->getMessageType() ) {
				case Message::CONTACT_ENTER: {
					std::shared_ptr<CMessageContactEnter> contactMsg = std::static_pointer_cast<CMessageContactEnter>(message);
					Logic::TEntityID idPlayerHit = contactMsg->getEntity();
					CEntity * playerHit = CServer::getSingletonPtr()->getMap()->getEntityByID(idPlayerHit);
					
					_explotionActive=true;
					// Si es el escudo del screamer mandar directamente esos da�os a la
					// entidad contra la que hemos golpeado (el escudo), sino, crear explosion
					if(playerHit->getType() == "ScreamerShield") {
						// Mandar mensaje de da�o
						// Emitimos el mensaje de da�o
						std::shared_ptr<CMessageDamaged> dmgMsg = std::make_shared<CMessageDamaged>();
						dmgMsg->setDamage(_explotionDamage);
						dmgMsg->setEnemy(_owner); // No tiene importancia en este caso
						playerHit->emitMessage(dmgMsg);

						// Crear efecto y sonido de absorcion

						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity,false);
					}
					else {
						// Eliminamos la entidad en diferido
						CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity,false);
						// Creamos la explosion
						createExplotion();
					}
					break;
				}
				case Message::CONTACT_EXIT: {
						std::cout << "CONTACT EXIT DE LA GRANADA, no deberia entrar nunca" << std::endl;
				}
			}
		}
	} // process

	//________________________________________________________________________

	void CGrenadeController::createExplotion() {
		// EntitiesHit sera el buffer que contendra la lista de entidades que ha colisionado
		// con el overlap
		CEntity** entitiesHit = NULL;
		int nbHits = 0;

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_explotionRadius);
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, _entity->getPosition(), entitiesHit, nbHits);

		// Mandamos el mensaje de da�o a cada una de las entidades que hayamos golpeado
		// Adem�s aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida
			// y no se trata del escudo
			if(entitiesHit[i] != NULL && entitiesHit[i]->getType() != "ScreamerShield") {
				//queremos que la explosion se reduzca con el tiempo, asi que calculamos
				//la distancia que hay entre el impacto y el jugador, despu�s partimos
				//la distancia maxima de impacto en cachitos de igual damage, y calculamos
				//cuantos cachitos de damage se le tienen que poner al hit
				Vector3 direccionImpacto = entitiesHit[i]->getPosition() - _entity->getPosition();
				float damage = (_explotionDamage/_explotionRadius)*direccionImpacto.length();

				//final damage
				damage = _explotionDamage - damage;

				//send damage message
				std::shared_ptr<CMessageDamaged> damageDone = std::make_shared<CMessageDamaged>();
				damageDone->setDamage(damage);
				damageDone->setEnemy(_owner);
				entitiesHit[i]->emitMessage(damageDone);
				
				// queremos que el desplazamiento sea m�s grande cuanto m�s cerca ha 
				//caido del objetivo (porque ha recibido m�s da�o y la onda expansiva
				//se reduce con la distancia, as� que calculamos cuanta fuerza le 
				//tenemos que dar al jugador en el desplazamiento de la explosi�n
				std::shared_ptr<CMessageAddForcePlayer> explotionForce = std::make_shared<CMessageAddForcePlayer>();
				// Seteamos la fuerza y la velocidad
				// Seteamos el vector director del desplazamiento
				
				Vector3 forceDirection = direccionImpacto.normalisedCopy();

				float pushForce=3;

				explotionForce->setForce(forceDirection*pushForce);
				entitiesHit[i]->emitMessage(explotionForce);
			}
		}

		// Limpiamos el buffer si es necesario
		if(nbHits > 0) delete [] entitiesHit;

		//Solo para singlePlayer, quitar al terminar
		//Sonido de explosion
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
		audioMsg->setRuta(_audioExplotion);
		audioMsg->setId("audioExplotion");
		audioMsg->setPosition(_entity->getPosition());
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(false);
		_entity->emitMessage(audioMsg);
		//Particulas
		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle("ExplosionParticle");
		particleMsg->setPosition(_entity->getPosition());
		_entity->emitMessage(particleMsg);
	} // createExplotion

	//________________________________________________________________________

	void CGrenadeController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

