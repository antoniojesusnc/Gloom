#ifndef __Logic_SpawnPlayer_H
#define __Logic_SpawnPlayer_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la muerte del jugador, y devolver� a la vida en una posici�n random dada por un manager de respawn al jugador pasado cierto tiempo.
	<p>
	Poseera un booleano que controla si la entidad esta muerta o no, adem�s del tiempo que le queda para spawnear otra vez.

*/
	class CSpawnPlayer : public IComponent
	{
		DEC_FACTORY(CSpawnPlayer);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CSpawnPlayer() : IComponent(){}
		

	
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		*/
		virtual void activate();

		/**
		Este componente s�lo acepta mensajes de tipo PLAYER_DEAD (por ahora).
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo PLAYER_DEAD desactivamos los componentes pertinentes y activamos el contador de tiempo para que
		en el tick si esta muerta y ha pasado cierto tiempo devolvamos a la vida al jugador.
		*/
		virtual void process(CMessage *message);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);
	

	protected:

		/**
		M�todo que hace las acciones correspondientes a una muerte.
		*/
		void dead();


		/**
		Nos lleva el estado de la entidad, viva(false) o muerta(true).
		*/
		bool _isDead;

		/**
		Nos lleva el tiempo que permanecer� muerta.
		*/
		unsigned int _timeSpawn;

		/**
		Nos lleva el tiempo que lleva muerta.
		*/
		unsigned int _actualTimeSpawn;

		/**
		Nos sirve para no actualizar continuamente el hud con el tiempo de spawn.
		*/
		int _timeToSendHud;

	}; // class CShoot

	REG_FACTORY(CSpawnPlayer);

} // namespace Logic

#endif // __Logic_SpawnPlayer_H
