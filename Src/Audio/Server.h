/**
@file Server.h

Contiene la declaraci�n de la clase CServer, Singleton que se encarga de
la gesti�n del audio en el juego.

@see Audio::CServer

@author Jose Antonio Garc�a Y��ez
@date Marzo, 2013
*/
#ifndef __Audio_Server_H
#define __Audio_Server_H

#include "fmod.hpp"
#include "fmod_errors.h"

#include <iostream>

using namespace FMOD;

// Declaraci�n de la clase
namespace Audio
{
	/**
	Servidor del m�dulo Audio que se encarga de la gesti�n del audio del juego. Est� 
	implementado como un singlenton de inicializaci�n expl�cita. Sirve 
	para comunicar a FMOD los eventos de sonido que tenga que reproducir.

	@author Jose Antonio Garc�a Y��ez
	@date Marzo, 2013
	*/
	class CServer
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� al m�todo tick() del mapa.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		Se encarga de cargar un sonido y reproducirlo en modo normal.
		*/
		void playSound(char* rutaSonido);

		/**
		Se encarga de cargar un sonido y reproducirlo en modo loop.
		*/
		void playLoopSound(char* rutaSonido);



	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		/**
		Gestiona los errores de inicializaci�n del sistema o carga/reproduccion de sonidos.
		*/
		void ERRCHECK(FMOD_RESULT result);


	private:
		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;

		/**
		Variable sistema de fmod.
		*/
		System* _system; // reminiscencias de C

	}; // class CServer

} // namespace Audio

#endif // __Audio_Server_H
