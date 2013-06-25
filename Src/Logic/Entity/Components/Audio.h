/**
@file Audio.h

Contiene la declaraci�n del componente que controla el sonido de la entidad.

@see Logic::CAudio
@see Logic::IComponent

@author Jose Antonio Garc�a Y��ez
@date Marzo, 2013
*/
#ifndef __Logic_Audio_H
#define __Logic_Audio_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla el audio de una entidad. Procesa mensajes de tipo 
	Audio y se encarga de comunicarle al servidor de audio la petici�n de sonido.
	
    @ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez
	@date Marzo, 2013
*/
	class CAudio : public IComponent
	{
		DEC_FACTORY(CAudio);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAudio() {};

		/**
		Este componente s�lo acepta mensajes de tipo AUDIO.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo AUDIO se encarga de comunicarselo al server de audio.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	// VACIO

	}; // class CAudio

	REG_FACTORY(CAudio);

} // namespace Logic

#endif // __Logic_Audio_H
