#include "MessageCameraToEnemy.h"

#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCameraToEnemy);

	CMessageCameraToEnemy::CMessageCameraToEnemy() : CMessage(TMessageType::CAMERA_TO_ENEMY) {
		// Nada que hacer
	}//
	//----------------------------------------------------------


	CEntity* CMessageCameraToEnemy::getEnemy(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageCameraToEnemy::setEnemy(CEntity* entity){
		_entity=entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageCameraToEnemy::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_entity));
		_tempBuffer->serialize(std::string("CMessageDamaged"));
		_tempBuffer->serialize(_entity);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCameraToEnemy::deserialize(Net::CBuffer& buffer) {
		int temp;
		buffer.deserialize(temp);

		// HAY QUE CONSTRUIR UNA ENTIDAD ENTITY - NO EXISTE METODO
		// setEntityID
	}

};
