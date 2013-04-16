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
	Componente que controla la vida y la armadura de una entidad.
	Procesa mensajes de da�o y de "tweaking".
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@author Jose Antonio Garc�a Y��ez
	@date Marzo, 2013
	*/
	
	class CLife : public IComponent {
		DEC_FACTORY(CLife);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CLife();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CLife();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		M�todo llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		par�metros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>defaultLife:</strong> Vida por defecto del personaje (depende de la clase de �ste). </li>
			<li><strong>damageOverTime:</strong> Cantidad de vida que se pierde con el tiempo. </li>
			<li><strong>damageTimeStep:</strong> Cada cuanto se resta vida de forma autom�tica. </li>
			<li><strong>shieldDamageAbsorption:</strong> Cantidad de da�o que absorbe el escudo (de 0 a 100). </li>
			<li><strong>maxLife:</strong> M�ximo de vida que puede tener el personaje. </li>
			<li><strong>maxShield:</strong> M�ximo de armadura que puede tener el personaje. </li>
			<li><strong>audioPain:</strong> Nombre del fichero de audio que se dispara al herir al personaje. </li>
			<li><strong>audioDeath:</strong> Nombre del fichero de audio que se dispara al morir el personaje. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de vida y escudo a los fijados por defecto.
		*/
		virtual void activate();

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>ADD_LIFE</li>
			<li>ADD_SHIELD</li>
			<li>SET_REDUCED_DAMAGE</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Resta los da�os indicados a la entidad (tanto de escudo
		como de vida), dispara los sonidos de da�o correspondientes
		y envia el mensaje de muerte si es necesario.

		@param damage Da�o que queremos restar al personaje.
		@param enemy Entidad que nos est� haciendo da�o.
		*/
		void damaged(int damage, CEntity* enemy);

		//__________________________________________________________________
		
		/**
		Suma la vida pasada por par�metro.

		@param life Cantidad de vida que queremos sumar.
		*/
		void addLife(int life);

		//__________________________________________________________________
		
		/**
		Suma los puntos de armadura pasados por par�metro.

		@param shield Puntos de armadura que queremos sumar.
		*/
		void addShield(int shield);

		//__________________________________________________________________

		/**
		Dado un porcentaje de reducci�n de da�os (comprendido entre
		0 y 1) reduce todo el da�o recibido en base a ese porcentaje.

		Por ejemplo, un porcentaje de 1 har�a que el personaje siempre
		recibiera un da�o de 0 puntos (o en otras palabras, que
		fuera inmune).

		@param percentage Porcentaje de reducci�n de da�os. Tiene que 
		estar comprendido entre 0 y 1.
		*/
		inline void reducedDamageAbsorption(float percentage);

		//__________________________________________________________________

		/**
		El personaje muere al ejecutarse este m�todo independientemente de
		la situaci�n o los powerups que tenga.
		*/
		void suicide();

	private:


		// =======================================================================
		//                          M�TODOS PRIVADOS
		// =======================================================================
		
		
		/**
		Resta los puntos de da�o indicados.

		@param damage Da�o que queremos restar a la entidad.
		@return true si el personaje ha muerto.
		*/
		bool updateLife(int damage);

		//__________________________________________________________________

		/**
		Setea la posici�n de la c�mara y env�a el mensaje de muerte.

		@param enemy Enemigo al que queremos que mire la c�mara.
		*/
		void triggerDeathState(CEntity* enemy);

		//__________________________________________________________________

		/**	Dispara el sonido de muerte. */
		void triggerDeathSound();

		//__________________________________________________________________

		/** Dispara el sonido de herido. */
		void triggerHurtSound();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** 
		Porcentaje de da�os recibidos. Su valor por defecto es 0 (desactivado), para hacer
		que el personaje sea inmune hay que fijarlo a 1. 
		*/
		float _reducedDamageAbsorption;

		/** Vida por defecto del personaje, depende de la clase. */
		int _defaultLife;

		/** Vida actual del personaje. */
		int _currentLife;
		
		/** Vida m�xima del personaje. */
		int _maxLife;

		/** Puntos de armadura que el personaje tiene. */
		int _currentShield;
		
		/**	Puntos m�ximos de armadura que el personaje puede tener. */
		int _maxShield;
		
		/** Porcentaje de da�o que absorbe la armadura. De 0 a 1. */
		float _shieldDamageAbsorption;

		/** Cantidad de vida que se le resta al personaje con el paso del tiempo. */
		int _damageOverTime;
		
		/** Cada cuanto tiempo baja la vida del personaje. */
		unsigned int _damageTimeStep;
		
		/** Timer que controla el tiempo que ha pasado desde el anterior step de da�o. */
		unsigned int _damageTimer;

		/** Ruta del sonido de da�o. */
		std::string _audioPain;

		/** Ruta del sonido de muerte. */
		std::string _audioDeath;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
