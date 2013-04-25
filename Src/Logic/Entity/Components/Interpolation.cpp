/**
@file Interpolation.cpp

@author Francisco Aisa Garc�a
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

#include "Interpolation.h"

#include "Logic/Entity/Entity.h"
#include "PhysicController.h"
#include "AvatarController.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

#include <math.h>

#include <iostream>

namespace Logic  {
	
	IMP_FACTORY(CInterpolation);

	//________________________________________________________________________

	bool CInterpolation::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		_speed = entityInfo->getFloatAttribute("maxVelocity");
		// Indicar parametros de interpolacion (�apeado de momento)
		_interpolating = false;
		_maxDistance = 15;
		_minDistance = 0.5f;
		_minYaw = 1.5;
		_maxYaw = 15;
		_yawDifference = 0;
		_rotationSpeed = 0.2;
		_serverDirection = Vector3(0,0,0);
		_distance = 0;
		_keyWS = 0;
		_keyAD = 0;
		_oldPos = _entity->getPosition();
		return true;
	} // spawn

	//________________________________________________________________________

	bool CInterpolation::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::SYNC_POSITION ||
				message->getMessageType() == Message::CONTROL;
	} // accept
	
	//________________________________________________________________________

	void CInterpolation::tick(unsigned int msecs){
		_oldPos = _entity->getPosition();
		_msecs = msecs;

		//si no estamos interpolando, gl
		//std::cout << "interpolamos? -> " << _interpolating << std::endl;
		//moveServerPos(msecs);
		if(!_interpolating)
			return;
		//std::cout << "interpolandooooo "<< std::endl;
		//lo primero de todo, movemos la posici�n del servidor para poder interpolar con m�s exactitud
		Vector3 newPos;
		//std::cout << "puedo interpolar? " << _canInterpolateMove << std::endl;
		if(_canInterpolateMove){
			//calculamos la direccion en la que debemos interpolar
			Vector3 direction = _serverDirection*Vector3(1,0,1);
		
			//calculamos el movimiento que debe hacer el monigote, mucho mas lento del que debe hacer de normal
			direction *=(_entity->getComponent<CAvatarController>("CAvatarController")->getVelocity()/5);
			
			//si nos hemos pasado, debemos moverlo al sitio
			if(direction.length() > _distance){
				direction*=(_distance/direction.length());
			}
			//std::cout << direction.length() << " contra " << _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity().length() << std::endl;
			//_entity->getComponent<CAvatarController>("CAvatarController")->addForce(Vector3(direction));
			_entity->getComponent<CPhysicController>("CPhysicController")->move(direction,msecs);
			//std::cout << "nueva pos lenght " << _distance << std::endl << std::endl;
			_distance -= direction.length();
			
		}//if
		if(_canInterpolateRotation){

			//si la diferencia es demasiado grande, lo movemos a pelo
			if(_yawDifference > _maxYaw || _yawDifference < _maxYaw*(-1)){
				Matrix3 a;
				_serverPos.extract3x3Matrix(a);
				_entity->setOrientation(a);
				_yawDifference = 0;
			}

			//si la diferencia no es demasiado grande, interpolamos
			else if(_yawDifference > _minYaw || _yawDifference < _minYaw*(-1)){

				//nos movemos el raton
				if(_yawDifference > _rotationSpeed){

				}else if(_yawDifference < _rotationSpeed*(-1)){
					_entity->yaw(_rotationSpeed);

				}else{
					_entity->yaw(_yawDifference);
					_yawDifference = 0;
				}//if(_yawDifference > _rotationSpeed)

			}//if
			//si la diferencia es peque�a no hacemos nada
			_canInterpolateRotation = false;
		}//if(_canInterpolateRotation)

		//si hemos terminado de interpolar, lo dejamos
		if((_distance < _minDistance) && ( _yawDifference < _minYaw || _yawDifference > _minYaw*(-1) )){
			_interpolating = false;
		}
		
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::process(const std::shared_ptr<CMessage>& message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			{
			std::shared_ptr<CMessageSyncPosition> syncMsg = std::static_pointer_cast<CMessageSyncPosition>(message);

			//std::cout << "el server me dice que esto en: " << syncMsg->getTransform().getTrans() << std::endl;

			//std::cout << "yo estoy en: " << _entity->getPosition() << std::endl;

			// nos guardamos la posi que nos han dado por si tenemos que interpolar
			_serverPos = syncMsg->getTransform();
			//calculo el ping que tengo ahora mismo
			int ping = clock()+Logic::CServer::getSingletonPtr()->getDiffTime()-syncMsg->getTime();
			_actualPing = abs(ping);
			//calculamos la interpolacion
			calculateInterpolation();

			break;
			}
			case Message::CONTROL: {
				std::shared_ptr<CMessageControl> controlMsg = std::static_pointer_cast<CMessageControl>(message);

				_canInterpolateMove = true;

				//calculamos la direcci�n del movimiento
				switch( controlMsg->getType() ) {
					case Control::WALK: {
						_keyWS+=1;
						break;
					}
					case Control::STRAFE_LEFT: {
						_keyAD+=1;
						_canInterpolateMove = true;
						break;
					}
					case Control::SIDEJUMP_LEFT: {
						_keyAD+=1;
						_canInterpolateMove = true;
						break;
					}
					case Control::STRAFE_RIGHT: {
						_keyAD-=1;
						break;
					}
					case Control::SIDEJUMP_RIGHT: {
						_keyAD-=1;
						break;
					}
					case Control::WALKBACK: {
						_keyWS-=1;
						break;
					}
					case Control::STOP_WALK: {
						_keyWS-=1;
						break;
					}
					case Control::STOP_WALKBACK: {
						_keyWS+=1;
						break;
					}
					case Control::STOP_STRAFE_LEFT: {
						_keyAD-=1;
						break;
					}
					case Control::STOP_STRAFE_RIGHT: {
						std::cout << "stop -> "<< _keyAD << std::endl;
						_keyAD+=1;
						break;
					}
					case Control::MOUSE: {
						std::shared_ptr<CMessageMouse> mouseMsg = std::static_pointer_cast<CMessageMouse>(message);
						Math::yaw( mouseMsg->getMouse()[0], _serverPos);
						_canInterpolateRotation = true;
						break;
					}
				}//switch

				break;
			}//case Message::CONTROL:

		}//switch

		//std::cout << "movimiento AD -> "<< _keyAD << std::endl;
		//std::cout << "movimiento WS -> "<< _keyWS << std::endl;
		//comprobamos si nos estamos moviendo, de manera que si no nos estamos moviendo no interpolamos
		if(_keyWS == 0 && _keyAD == 0){
			_canInterpolateMove = false;
		}

		//comprobamos si es el jugador local, de manera que si no lo es le decimos que puede interpolar 
		//siempre que deba hacerlo
		if(!_entity->isPlayer())
			_canInterpolateMove = true;
	} // process

////////////////////////////////////////////////////////////////////////////////////////////////////////

/*	void CInterpolation::moveServerPos(unsigned int msecs){

		//primero movemos hacia adelante o hacia atr�s
		if(_serverDirection != Vector3(0,0,0)){
			Vector3 newPos = _serverPos.getTrans();
			//nueva posi = (old posi + direcion*orientacion)*velocidad
			//std::cout << "server pos antes " << _serverPos.getTrans() << std::endl;
			newPos+=_serverDirection.normalisedCopy()*_speed*Math::getDirection(_serverPos)*msecs;
			//std::cout << "server pos mientras " << newPos << std::endl;
			_serverPos.setTrans(newPos);
		}
		
		//ahora nos movemos en la direcci�n del strafe
		if(_serverStrafeDirection != Vector3(0,0,0)){
			Matrix4 strafe = _serverPos;
			//std::cout << "direccion en INTERPOLATION 1 " << _serverStrafeDirection.normalisedCopy()*Math::getDirection(strafe) <<  std::endl;
			Math::yaw(Math::PI*0.5, strafe);
			Vector3 newPos = strafe.getTrans();
			//nueva posi = (old posi + direcion*orientacion)*velocidad
			//std::cout << "server pos antes " << _serverPos.getTrans() << std::endl;
			newPos+=_serverStrafeDirection.normalisedCopy()*_speed*Math::getDirection(strafe)*msecs;
			//std::cout << "server pos mientras " << newPos << std::endl;
			_serverPos.setTrans(newPos);
			//std::cout << "direccion en INTERPOLATION 2 " << _serverStrafeDirection.normalisedCopy()*Math::getDirection(strafe) <<  std::endl << std::endl;
		}
	}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::calculateInterpolation(){
		
		//primero debemos setear la posici�n del servidor, suponiendo donde debe estar en el tiempo
		//que ha tardado la informaci�n en llegar hasta aqu�
		Vector3 serverPos = _serverPos.getTrans();
		Vector3 serverDisplacement = _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity();
		//serverDisplacement*=_actualPing/_msecs;

		//esta es la posi que suponemos que tiene el server en eeste momento
		serverPos+=serverDisplacement;

		//direccion de interpolaci�n
		Vector3 intDirection = serverPos - _entity->getPosition();

		float distance = intDirection.length();

		//std::cout << "estoy en " << _entity->getPosition() << */" y el server en " << _serverPos.getTrans() << std::endl;
		std::cout << "supongo que el server esta en " << _actualPing/_msecs << std::endl << std::endl;

		if(distance > _maxDistance){
			_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(_serverPos.getTrans());
			//Movemos la orientacion logica/grafica
			Matrix3 tmpMatrix;
			_serverPos.extract3x3Matrix(tmpMatrix);
			_entity->setOrientation(tmpMatrix);
			return;
		}
		//si la distancia es mayor que min distance y menor que maxDistance interpolamos
		else if(distance > _minDistance){
			//calculamos donde nos deber�a tener el server mas o menos

			_serverDirection = intDirection.normalisedCopy();
			_distance = distance;

			_interpolating = true;

		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CInterpolation::activate(){
		IComponent::activate();
		_serverDirection = Vector3(0,0,0);
		_interpolating = false;
		_distance = 0;
	}

} // namespace Logic

