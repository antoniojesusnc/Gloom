/**
@file ErrorManager.h

Contiene la declaraci�n del gestor de errores para PhysX.

@see Physics::CErrorManager

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_ErrorManager_H
#define __Physics_ErrorManager_H

#include <foundation/PxErrorCallback.h>

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Gestor de errores para PhysX. Cuando PhysX detecta que se intenta hacer algo incorrecto 
	(por ejemplo, llamadas a funciones con par�metros no v�lidos), invoca a este gestor 
	que a su vez imprime el mensaje en la consola.

	@ingroup physicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CErrorManager : public physx::PxErrorCallback {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CErrorManager();

		//________________________________________________________________________

		/** Destructor. */
		~CErrorManager();


		// =======================================================================
		//                   METODOS HEREDADOS DE PxErrorCallback
		// =======================================================================


		/** 
		M�todo invocado por PhysX cada vez que se produce un error. El mensaje de error se 
		imprime por pantalla.

		@param code C�digo del error.
		@param message Mensaje a imprimir.
		@param file Fichero en el que ocurri� el error.
		@param line L�nea donde ocurri� el error.
		*/
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

	}; // classCErrorManager

}; // namespace Physics

#endif // __Physics_ErrorManager_H