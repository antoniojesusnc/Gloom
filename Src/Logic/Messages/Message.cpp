/**
@file Message.cpp

Contiene la implementaci�n de la clase Message y sus clases hijas, que ser�n los mensajes. Es una colecci�n de componentes.

@author Jose Antonio Garc�a Y��ez
@date Enero, 2013
*/

#include "Message.h"

namespace Logic 
{

	CMessage::~CMessage() {
		if(_tempBuffer != NULL)
			delete _tempBuffer;
	}

	TMessageType CMessage::getMessageType(){
		return _type;
	}//
	//----------------------------------------------------------

	CMessage::CMessage(TMessageType t) : _type(t), _smartP(0), _tempBuffer(NULL) {
		// Nada que hacer
	}
	
}

	
