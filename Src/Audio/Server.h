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
#include "BaseSubsystems/Math.h"

#include <iostream>
#include <string>
#include <map>

using namespace FMOD;

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
}

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
		Se encarga de cargar un sonido (no 3D) y reproducirlo en modo normal.
		*/
		void playSound(const std::string& soundName, bool loopSound = false, bool streamSound = false);

		/**
		Se encarga de cargar un sonido 3D y reproducirlo en modo loop.
		*/
		void playSound3D(const std::string& soundName, const Vector3& position, const Vector3& speed, bool loopSound = false, bool streamSound = false);

		/**
		Se encarga de parar un sonido introduciendo su nombre como par�metro.

		@param id El identificador del sonido
		*/
		void stopSound(const std::string& soundName);

		/**
		Se encarga de parar todos los sonidos
		*/
		void stopAllSounds();

		/**
		Se encarga de mutear el sonido que se reproduce por los canales.
		*/
		void mute();

		/**
		Establece el componente del jugador con el que preguntaremos la posici�n para actualizar la posici�n de escucha.

		@param controlledAvatar Componente al que le preguntaremos la posici�n de la entidad.
		*/
		void setSoundAvatar(Logic::CEntity *controlledAvatar) 
										{_soundAvatar = controlledAvatar;};

	protected:

		/**
		Constructor.
		*/
		CServer();

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

		/**
		Guarda las asociaciones de nombreSonido/Canal
		*/
		typedef std::map<std::string, Channel *> SoundChannelMap;
		SoundChannelMap _soundChannel;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;

		/**
		Variable sistema de fmod.
		*/
		System* _system; // reminiscencias de C

		/**
		Variable que controla el volumen de los sonidos por defecto.
		*/
		float _volume;

		/**
		Variable que define el tiempo m�nimo para que se ejecute el tick del servidor.
		*/
		unsigned int _minimumExecuteTime;

		/**
		Variable que lleva el tiempo para ejecutar el tick del servidor.
		*/
		unsigned int _timeToExecute;

		/**
		Factores doppler y rolloff del sistema
		*/
		float _doppler; 
		float _rolloff;

		/**
		Player(Protagonista) del audio.
		*/
		Logic::CEntity* _soundAvatar;
		short _playerHeight;

		/**
		Booleano que controla si el server esta muteado.
		*/
		bool _isMute;

		/** Ruta por defecto a la biblioteca de sonidos usados por el juego. */
		std::string _audioResourcesPath;

	}; // class CServer

} // namespace Audio

#endif // __Audio_Server_H
