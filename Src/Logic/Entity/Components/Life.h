/**
@file Life.h

Contiene la declaraci�n del componente que 
controla la vida de un personaje.

@see Logic::CLife
@see Logic::IComponent

@author Francisco Aisa Garc�a
@author Jose Antonio Garc�a Y��ez
@date Marzo, 2013
*/

#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el da�o a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animaci�n de muerte?)
	y si es el jugador habr� que terminar el juego. Si la vida sigue siendo mayor 
	que 0 tr�s un golpe �poner la animaci�n de herido?.
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@author Jose Antonio Garc�a Y��ez
	@date Marzo, 2013
	*/
	
	class CLife : public IComponent {
		DEC_FACTORY(CLife);
	public:

		/** Constructor por defecto; en la clase base no hace nada. */
		CLife();

		/** Destructor. */
		virtual ~CLife();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void activate();

		/**
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		/**
		Al recibir da�o quitamos vida
		*/
		void damaged(int damage, CEntity* enemy);
		
		/**
		Sumar vida 
		*/
		void addLife(int life);
		
		/**
		Sumar escudo
		*/
		void addShield(int shield);

		// Activa la inmunidad
		void setImmunity(bool isImmune);

		// Numero entre 0 y 1
		void reducedDamageAbsorption(float percentage);

	private:

		bool updateLife(int damage);

		void triggerDeathState(CEntity* enemy);

		void triggerDeathSound();

		void triggerHurtSound();

		bool _isImmune;

		float _reducedDamageAbsorption;

		/**
		Vida por defecto del personaje, depende de la clase.
		*/
		int _defaultLife;

		/**
		Vida actual de la entidad
		*/
		int _currentLife;
		
		/**
		M�xima vida de la entidad
		*/
		int _maxLife;

		/**
		Escudo de la entidad
		*/
		int _currentShield;
		
		/**
		M�ximo escudo de la entidad
		*/
		int _maxShield;
		
		/**
		Porcentaje de da�o que absorve el escudo a la entidad
		*/
		int _shieldDamageAbsorption;


		/** Cantidad de vida que se le resta al personaje con el paso del tiempo. */
		int _damageOverTime;
		
		/** Cada cuanto tiempo baja la vida del personaje. */
		unsigned int _damageTimeStep;
		
		/** Timer que controla el tiempo que ha pasado desde el anterior step de da�o. */
		unsigned int _damageTimer;


		/**
		Indica si el jugador est� muerto.
		*/
		bool _playerIsDead;

		/**
		Ruta del sonido de da�o
		*/
		std::string _audioPain;

		/**
		Ruta del sonido de muerte
		*/
		std::string _audioDeath;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
