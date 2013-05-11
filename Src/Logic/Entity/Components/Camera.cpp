/**
@file Camera.cpp

Contiene la implementaci�n del componente que controla la c�mara gr�fica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

#include "Interpolation.h"
#include "AvatarController.h"

#include "Logic/Messages/MessageCameraToEnemy.h"
#include "Logic/Messages/MessageCameraOffset.h"
#include "Logic/Messages/MessageCameraRoll.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	CCamera::CCamera() : _graphicsCamera(0), 
						 _height(8), 
						 _currentRoll(0.0f), 
						 _verticalOffset(0),
						 _offset(Vector3::ZERO) {

		// Nada que hacer
	}

	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		Vector3 camPos;
		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");
		if(entityInfo->hasAttribute("target"))
			_targetV = entityInfo->getVector3Attribute("target");
		if(entityInfo->hasAttribute("position")){
			camPos = entityInfo->getVector3Attribute("position");
			_graphicsCamera->setCameraPosition(camPos);
		}
		_dead=false;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CCamera::onActivate() {
		_target = CServer::getSingletonPtr()->getPlayer();

		if(!_target){
			_graphicsCamera->setTargetCameraPosition(_targetV);
			_target = NULL;
			//deactivate();
		}

		_fOffsetTimer = 0.0f;
		_fRoll = 0.0f;

		//return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::onDeactivate() {
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	bool CCamera::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		
		return msgType == Message::CAMERA_TO_ENEMY ||
			   msgType == Message::PLAYER_SPAWN || 
			   msgType == Message::CAMERA_OFFSET || 
			   msgType == Message::CAMERA_ROLL;
	} // accept
	//---------------------------------------------------------


	void CCamera::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CAMERA_TO_ENEMY: {
				std::shared_ptr<CMessageCameraToEnemy> cameraToEnemyMsg = std::static_pointer_cast<CMessageCameraToEnemy>(message);
				setTargetEnemy( cameraToEnemyMsg->getEnemy() );
				//std::cout << "MENSAJE DE CAMERATOENEMY: " << std::endl;
				break;
			}
			case Message::PLAYER_SPAWN: {
				_dead=false;
				//std::cout << "mensaje respawn recibido" << std::endl;
				break;
			}
			case Message::CAMERA_OFFSET: {
				std::shared_ptr<CMessageCameraOffset> camOffset = std::static_pointer_cast<CMessageCameraOffset>(message);
				_fOffsetTimer = camOffset->getOffsetTimer(); //asignamos el tiempo del offset
				//_bCameraOffset = true;
				//_fOffsetTimer = 2000.0f; 
				//std::shared_ptr<CMessageCameraToEnemy> cameraToEnemyMsg = std::static_pointer_cast<CMessageCameraToEnemy>(message);
				break;
			}
			case Message::CAMERA_ROLL: {
				std::shared_ptr<CMessageCameraRoll> camOffset = std::static_pointer_cast<CMessageCameraRoll>(message);
				_fRoll = camOffset->getRollDegrees(); //asignamos el tiempo del offset
			}
		}

	} // process
	//---------------------------------------------------------

	void CCamera::rollCamera(float radians) {
		_graphicsCamera->rollCamera( (-1 * _currentRoll) + radians );
		_currentRoll = radians;
	}

	float CCamera::getRoll() {
		return _currentRoll;
	}

	void CCamera::setTargetEnemy(CEntity* enemy){
		
		this->_enemy=enemy;
		//std::cout << "el enemigo a apuntar se llama " << _enemy->getName() << std::endl;
		//td::cout << "y su tipo es " << _enemy->getType() << std::endl;
		_dead=true;
	}

	//---------------------------------------------------------

	void CCamera::setPosition(const Vector3& position) {
		_graphicsCamera->setCameraPosition(position);
	}

	//---------------------------------------------------------

	Vector3 CCamera::getPosition() {
		return _graphicsCamera->getCameraPosition();
	}

	//---------------------------------------------------------

	void CCamera::setVerticalOffset(float y) {
		_offset.y = y;
	}

	void CCamera::setOffset(const Vector3& offset) {
		this->_offset = offset;
	}

	void CCamera::addOffset(const Vector3& offset) {
		this->_offset += offset;
	}

	Vector3 CCamera::getOffset() const {
		return _offset;
	}

	//---------------------------------------------------------

	void CCamera::onFixedTick(unsigned int msecs) {
		if(_target) {
			// Actualizamos la posici�n de la c�mara.
			Vector3 position = _target->getPosition();
			position.y += _height;
			position += _offset; // Seteamos el offset de la camara

			//Si hay offset (vibraci�n) de c�mara
			if (_fOffsetTimer > 0) {
				//Actualizo el timer
				_fOffsetTimer -= msecs;

				//"Vibraci�n" de la c�mara
				Matrix4 transf = _entity->getTransform();
 				Math::yaw(Math::HALF_PI, transf);
				_entity->setTransform(transf);
				
				//En el eje de movimiento horizontal
				Vector3 directionStrafe = Math::getDirection(_entity->getYaw() + Math::PI/2);
				position += directionStrafe;
			}

			//Ajustamos el roll si lo hay
			/*if ((_fRoll > 0.01f) || (_fRoll < 0.01f)) //Al ser un float no puedo comparar con 0.0f
			{
				_graphicsCamera->rollCamera(_fRoll);
				_fRoll = 0.0f; //Inicializamos el roll para que en el siguiente tick no entre
			}*/

			_graphicsCamera->setCameraPosition(position);
			
			if(!_dead){
				// Y la posici�n hacia donde mira la c�mara.
				Vector3 direction = Math::getDirection(_target->getOrientation());
				_graphicsCamera->setTargetCameraPosition(position + direction);
			}
			//Si estamos muertos miramos al enemigo, diferenciamos entre nosotros mismos o el rival
			else if(_enemy){
				 if(_enemy->getType().compare("LocalPlayer")!=0)
				    _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 else{	
					 _graphicsCamera->setCameraPosition(_enemy->getPosition()+Vector3(0,50,0));
					 _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 }
			}

		}else{
			_graphicsCamera->setTargetCameraPosition(_targetV);
		}
	} // tick


} // namespace Logic

