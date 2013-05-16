#include "MessagePositionSnapshot.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePositionSnapshot);

	//__________________________________________________________________

	CMessagePositionSnapshot::CMessagePositionSnapshot() : CMessage(Message::POSITION_SNAPSHOT) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessagePositionSnapshot::~CMessagePositionSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessagePositionSnapshot::setBuffer(const std::vector<Vector3> buffer) {
		_positionBuffer = buffer;
	}

	//__________________________________________________________________
		
	std::vector<Vector3> CMessagePositionSnapshot::getBuffer() {
		return _positionBuffer;
	}

	//__________________________________________________________________
		
	Net::CBuffer* CMessagePositionSnapshot::serialize() {
		assert(_tempBuffer == NULL);

		// Tama�o igual = cabecera(int) + tambuffer(int) + num matrices (5 floats * tamBuffer)
		int bufferSize = _positionBuffer.size();
		_tempBuffer = new Net::CBuffer( (2 * sizeof(int)) + (bufferSize * 3 * sizeof(float)) );
		_tempBuffer->serialize(std::string("CMessagePositionSnapshot"), true);
		_tempBuffer->serialize(bufferSize);
		for(int i = 0; i < bufferSize; ++i) {
			_tempBuffer->serialize(_positionBuffer[i]);
		}
		
		return _tempBuffer;
	}

	//__________________________________________________________________

	void CMessagePositionSnapshot::deserialize(Net::CBuffer& buffer) {
		int bufferSize;
		// Deserializar el tama�o del buffer
		buffer.deserialize(bufferSize);

		// Resize del buffer al tama�o leido
		_positionBuffer.clear();
		_positionBuffer.resize(bufferSize);
		// Leer tantas matrices como nos diga el tama�o
		for(int i = 0; i < bufferSize; ++i) {
			buffer.deserialize(_positionBuffer[i]);
		}
	}

};
