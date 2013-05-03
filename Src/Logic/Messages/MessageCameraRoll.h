#ifndef __Logic_MessageCameraRollEntity_H
#define __Logic_MessageCameraRollEntity_H

#include "Message.h"

namespace Logic {

	class CMessageCameraRoll: public CMessage{
	DEC_FACTORYMESSAGE(CMessageCameraRoll);
	public:
		CMessageCameraRoll();
		virtual ~CMessageCameraRoll();

		CEntity* getRelatedEntity();

		float getRollDegrees			()					{ return _iRollDegrees; }
			void setRollDegrees			(int iDegrees);
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	private:
		CEntity* _entity;

		/**
			Grados que se rotar� la c�mara.
			Negativo = Hacia la izquierda ; Positivo = Hacia la derecha
		*/
		int	_iRollDegrees; 
	};
	REG_FACTORYMESSAGE(CMessageCameraRoll);
};

#endif