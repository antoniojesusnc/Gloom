#include "MessageDamaged.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDamaged);

	CMessageDamaged::CMessageDamaged() : CMessage(TMessageType::DAMAGED) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setDamage(unsigned char damage){
		_damage = damage;
	}//
	//----------------------------------------------------------

	unsigned char CMessageDamaged::getDamage(){
		return _damage;
	}//
	//----------------------------------------------------------

	CEntity* CMessageDamaged::getEnemy(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setEnemy(CEntity* entity){
		_entity=entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageDamaged::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_damage) + sizeof(_entity));
		_tempBuffer->serialize(std::string("CMessageDamaged"),true);
		_tempBuffer->serialize(_damage);
		_tempBuffer->serialize(_entity->getEntityID());
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_damage);
		TEntityID id;
        buffer.deserialize(id);
	}

};
