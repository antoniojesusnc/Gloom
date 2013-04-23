/**
@file ShootRocketLauncher.cpp

Contiene la implementacion del componente
de disparo del lanzacohetes.

@see Logic::CShootRocketLauncher
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Abril, 2013
*/

#include "ShootRocketLauncher.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "RocketController.h"
#include "Logic/GameNetMsgManager.h"
#include "../../../Net/Manager.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"

namespace Logic {
	IMP_FACTORY(CShootRocketLauncher);

	CShootRocketLauncher::~CShootRocketLauncher() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShootRocketLauncher::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootProjectile::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;
		std::string weapon = aux.str();

		_shootForce = entityInfo->getFloatAttribute(weapon + "ShootForce");

		return true;
	}

	//__________________________________________________________________

	void CShootRocketLauncher::fireWeapon() {
		// Obtenemos la informacion asociada al arquetipo del cohete
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Rocket");

		//Paso1
		//Lo primero es ver el punto exacto en el que saldr�a el centro del cohete de la capsula
		//Sacamos un punto en la direcci�n que apuntamos muy fuera de la capsula y trazamos un raycast para el cual solo nos interesa en que punta choca con nosotros



		//Paso2
		//Separamos el cohete de la capsula en la direccion en la que se apuntaba



		//Paso3
		//Ahora creamos mediante overlap el cohete y nos aseguramos que no toque con nada
		//Con la separacion anterior no nos puede tocar a nosotros y si toca cualquier otra significa que no tiene espacio
		//Si no tiene espacio tenemos que crear el cohete solo graficamente y automaticamente explotarlo
		//Para ello creo que una nueva entidad(sin fisicas) con un componente ExplotionRocketServer/Client
		//Para red el server tendria el tipo server que ademas aplica da�o, y el cliente tendria solo lo gr�fico




		//Paso4
		//Si el overlap fue bien y no tocamos con nada, es el momento de crear el cohete en la posicion validada
		//Ojo con las separaciones extra por el rollo de la mierder interpolation y el papapatr�s ( lo mismo hay que modificar la posicion del paso 2 )




		//std::cout << "Posicion del jugador x,y,z: " << _entity->getPosition().x << "," << _entity->getPosition().y <<"," << _entity->getPosition().z << std::endl;
		//Calculamos la situacion de origen del cohete
		Vector3 directionNormalise=Math::getDirection( _entity->getOrientation());
		directionNormalise.normalise();
		Vector3 shootPosition = _entity->getPosition() + directionNormalise* (_capsuleRadius+6.0);//2 es el radio del cohete y lo demas es la separacion para que vaya tanto en sp como mp (basura de interpolacion)
		shootPosition.y += _heightShoot; //Altura del pj menos el radio del cohete para que salga en el centro de la mira
		//std::cout << "Posicion del cohete a disparar x,y,z: " << shootPosition.x <<"," << shootPosition.y <<"," << shootPosition.z << std::endl;

		//Comprobamos si el misil tiene espacio para ser disparado
		//Creamos el origen del rayo que sera igual al de la posicion de disparo menos el desplazamiento
		Vector3 origin = _entity->getPosition()+Vector3(0,_heightShoot,0);
		Vector3 noSpacePosition=origin;
		//Calculamos la distancia entre la posicion de disparo y el origen
		float distance=origin.distance(shootPosition);
		// Creamos el ray desde el origen en la direccion del raton
		Vector3 direction=Math::getDirection(_entity->getOrientation());
		direction.normalise();
		Ray ray(origin, direction);
		//Desde el centro del jugador en la direcci�n que ser� disparado el misil sin tenernos en cuenta a nosotros mismos.
		//Si hemos tocado algo es que no hay espacio por lo tanto lo lanzamos desde el centro del jugador (creo que seria mejor explotarlo directamente por movidas de shapes)
		if(Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, distance,_entity->getEntityID()) != NULL){
			std::cout << "El cohete no tiene espacio para salir"<< std::endl;
			shootPosition=noSpacePosition;
		}

		float yaw=Math::fromRadiansToDegrees(_entity->getYaw())+180; //+180 porque esta del reves el modelo cohete	
		float pitch=360-Math::fromRadiansToDegrees(_entity->getPitch());

		CEntity* rocket = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
			entityInfo, Logic::CServer::getSingletonPtr()->getMap(), shootPosition, yaw, pitch);
		
		assert(rocket != NULL);
		
		rocket->activate();

		CRocketController* comp = rocket->getComponent<CRocketController>("CRocketController");
		assert(comp != NULL);
		// Seteamos la entidad que dispara el cohete
		comp->setOwner(_entity);

		// Mandar mensaje add force
		std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation()) * _shootForce), Physics::ForceMode::eFORCE );
		forceMsg->setGravity(false);
		rocket->emitMessage(forceMsg);
	}


} // namespace Logic

