/**
@file Component.h

Contiene la declaraci�n de la clase base de los componentes.

@see Logic::IComponent
@see Logic::CCommunicationPort

@author Francisco Aisa Garc�a
@date Abril, 2013
*/

#ifndef __Logic_Component_H
#define __Logic_Component_H

#include "CommunicationPort.h"
#include "Entity.h"
#include "Logic/Maps/ComponentFactory.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map {
	class CEntity;
}

namespace Logic {
	class CMap;
}

// Declaraci�n de la clase
namespace Logic {

	/**
	Clase base de los componentes que forman las entidades.
	<p>
	Un componente puede recibir mensajes y reaccionar ante ellos
	enviando otros mensajes al resto de componentes hermanos o
	realizando alguna acci�n sobre el entorno.
	<p>
	Dependiendo del estado en el que se encuentre el componente
	y de si ha redefinido o no los m�todos de onTick y onFixedTick
	la entidad invoca al m�todo que corresponda en cada frame.

	onTick y onFixedTick se llaman en cada frame con la diferencia
	de que onFixedTick siempre se llama con una cantidad de msecs
	constantes (tantas veces como haga falta para procesar los msecs
	que realmente han transcurrido en ese frame).

	El ciclo de vida de un componente t�pico viene determinado por
	el ciclo de vida de la entidad a la que pertenece:
	<ol>
		<li>
			Cuando se crea la entidad se crea el componente.
		</li>
		<li>
			Tras la llamada al constructor se llama al spawn,
			que es el m�todo de donde podemos leer los par�metros
			del mapa/arquetipos.
		</li>
		<li>
			Al cargarse el mapa se activan todas las entidades y con
			ello todos los componentes de la entidad (onActivate se invoca).
		</li>
		<li>
			Antes de comenzar con los ticks se invoca al start de la entidad
			y de los componentes (onStart en este caso).
		</li>
		<li>
			Durante la partida se llama periodicamente al m�todo onTick o
			onFixedTick de los componentes.
		</li>
		<li>
			Al salir de la partida o desactivar la entidad se desactivan
			todos sus componentes (onDeactivate se invoca).
		</li>
		<li>
			Al borrar el mapa se eliminan todas las entidades y se ejecuta
			el destructor de todos los componentes.
		</li>
	</ol>

	A groso modo el componente puede estar activado o desactivado.
	Si el componente est� desactivado ni procesa mensajes ni tiene tick.
	Sin embargo, si el componente est� activado puede encontrarse
	en uno de los siguientes estados:
	<ul>
		<li>
			<strong>AWAKE/AVAILABLE: </strong> El componente est� 
			despierto/disponible. Recibe mensajes y tiene tick.
		</li>
		<li>
			<strong>SLEEPING: </strong> El componente est� dormido.
			No tiene tick pero recibe mensajes. Al recibir mensajes
			vuelve a tener tick y pasa al estado AWAKE/AVAILABLE.
		</li>
		<li>
			<strong>BUSY: </strong> El componente est� ocupado.
			Tiene tick pero no recibe mensajes.
		</li>
	</ul>
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans� Garc�a
	@date Julio, 2010
	*/

	class IComponent : public CCommunicationPort {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		IComponent();

		//__________________________________________________________________

		/**
		Destructor (virtual); en la clase base no hace nada.
		*/
		virtual ~IComponent();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================
		

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). El m�todo es extendido
		en las clases hijas, que hacen cosas m�s inteligentes, dependiendo
		del tipo de componente concreto, pero siempre deber�n invocar a esta
		antes de hacer nada.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________
		
		/**
		M�todo llamado una �nica vez tras la activaci�n del componente. Se
		encarga de invocar a onStart.
		*/
		inline void start() { onStart(); }

		//__________________________________________________________________

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		Los milisegundos dados son variables.

		Se encarga de llamar a onTick que es el m�todo que realmente ejecuta 
		el comportamiento de los componentes derivados.

		@param msecs Milisegundos transcurridos desde el �ltimo tick (variable).
		*/
		inline void tick(unsigned int msecs) { onTick(msecs); }

		//__________________________________________________________________
		
		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		Se encarga llamar al m�todo que realmente tienen que redefinir los 
		componentes (onFixedTick).

		IMPORTANTE: Se diferencia del tick, en que msecs siempre es constante.

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Siempre
		son constantes.
		*/
		inline void fixedTick(unsigned int msecs) { onFixedTick(msecs); }
		
		//__________________________________________________________________

		/**
		Si el componente estaba desactivado lo activa. Si se lleva a cabo
		la activaci�n invoca al m�todo onActivate para que los componentes
		hijos hagan lo que necesiten en el evento de activaci�n.
		*/
		void activate();

		//__________________________________________________________________

		/**
		Si el componente estaba activado lo desactiva. Si se lleva a cabo
		la desactivaci�n invoca al m�todo onDeactivate para que los componentes
		hijos hagan lo que necesiten en el evento de desactivaci�n.
		*/
		void deactivate();

		//__________________________________________________________________

		/**
		Pone el componente a dormir (deja de tener tick). Si el componente 
		recibe un mensaje en este estado se despierta. Tambi�n podemos 
		despertarlo explicitamente llamando a wakeUp.

		Invoca al m�todo onSleep para que el componente haga lo que necesite
		al ponerse a dormir.
		
		@param deepSleep true si no queremos que el componente se despierte
		al recibir mensajes.
		*/
		void putToSleep(bool deepSleep = false);

		//__________________________________________________________________

		/**
		Despierta al componente e invoca al m�todo onWake SOLO si el componente
		estaba dormido.
		*/
		void wakeUp();

		//__________________________________________________________________

		/**
		Pone el componente en estado ocupado (deja de recibir mensajes pero
		tiene tick). Invoca al m�todo onBusy.

		La �nica forma de sacar al componente de este estado es llamando a
		stayAvailable.
		*/
		void stayBusy();

		//__________________________________________________________________

		/**
		Pone el componente en estado disponible (realmente lo que hace es
		despertarlo ya que disponible y despierto son el mismo estado) e
		invoca al m�todo onAvailable SOLO si el componente estaba en el estado
		BUSY.
		*/
		void stayAvailable();

		//__________________________________________________________________

		/** 
		Devuelve true si el componente est� activado. 

		@return true si el componente est� activado.
		*/
		inline bool isActivated() const { return _isActivated; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente est� ocupado. 
		
		@return true si el componente est� ocupado.
		*/
		inline bool isBusy() const { return _state == ComponentState::eBUSY; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente est� despierto/disponible. 
		
		@return true si el componente est� despierto.
		*/
		inline bool isAwake() const { return _state == ComponentState::eAWAKE; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente est� durmiendo. 

		@return true si el componente est� durmiendo.
		*/
		inline bool isSleeping() const { return _state == ComponentState::eSLEEPING; }

		//__________________________________________________________________

		/**
		M�todo que devuelve la entidad a la que pertenece el componente.

		@return La entidad a la que pertenece el componente.
		*/
		inline CEntity* getEntity() const { return _entity; }

	
	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================


		/**
		Se ejecuta la primera vez que la entidad se activa. Garantiza que todas las 
		entidades (incluidos sus componentes) han ejecutado el spawn y est�n listas
		para hacer el tick.
		*/
		virtual void onStart();

		//__________________________________________________________________

		/**
		Llamado en cada frame por fixedTick. Los clientes que hereden de esta
		clase deben redefinir su comportamiento.

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Siempre
		son constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		//__________________________________________________________________

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deber�n sobreescribir este m�todo con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Invocado cuando se activa el componente. Los componentes derivados
		deber�n redefinir este m�todo para hacer sus labores de activaci�n.
		*/
		virtual void onActivate();

		//__________________________________________________________________

		/**
		Invocado cuando se desactiva el componente. Los componentes derivados
		deber�n redefinir este m�todo para hacer sus labores de desactivaci�n.
		*/
		virtual void onDeactivate();

		//__________________________________________________________________

		/**
		Invocado al poner a dormir un componente. 
		*/
		virtual void onSleep();

		//__________________________________________________________________

		/**
		Invocado al despertar un componente.
		*/
		virtual void onWake();

		//__________________________________________________________________

		/**
		Invocado al poner un componente como ocupado.
		*/
		virtual void onBusy();

		//__________________________________________________________________

		/**
		Invocado al poner un componente como disponible.
		*/
		virtual void onAvailable();

		//__________________________________________________________________

		/**
		M�todo que establece la entidad a la que pertenece el componente.

		@param entity Entidad a la que pertenece el componente.
		*/
		void setEntity(CEntity *entity) { _entity = entity; }


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		/**
		Entidad que contiene al componente. Para reenviar el mensaje a 
		los otros componentes
		*/
		CEntity *_entity;


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** clase amiga que puede establecerse como poseedor del componente. */
		friend class CEntity;

		/** true si el componente esta activado. */
		bool _isActivated;

		/** 
		Enumerado que indica el estado en el que se encuentra el componente:
		<ul>
			<li>eAWAKE: El componete est� despierto/disponible.</li>
			<li>eBUSY: El componente est� ocupado.</li>
			<li>eSLEEPING: El componente est� durmiendo.</li>
		</ul>
		*/
		ComponentState::Enum _state;

		/** 
		Enumerado que indica el tipo de tick que tiene el componente: 
		<ul>
			<li>eNONE: No tiene tick de ning�n tipo.</li>
			<li>eTICK: Tiene tick.</li>
			<li>eFIXED_TICK: Tiene fixed tick.</li>
			<li>eBOTH: Tiene tick y fixed tick.</li>
		</ul>
		*/
		TickMode::Enum _tickMode;

		/** 
		true si el componente no se va a despertar al recibir 
		mensajes cuando este dormido 
		*/
		bool _deepSleep;
		
	}; // class IComponent


/////////////////////////////////////////////////////////////
// Macros para la adici�n de los componentes a la factor�a // 
// de componentes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaraci�n de los m�todos necesarios para que 
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define DEC_FACTORY(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static IComponent* create(); \
	/** \
	Registra el componente de la clase en la factor�a. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementaci�n de los m�todos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define IMP_FACTORY(Class) \
IComponent* Class::create() \
{ \
	IComponent* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	if (!CComponentFactory::getSingletonPtr()->has(#Class)) \
	{ \
		CComponentFactory::getSingletonPtr()->add(Class::create, #Class); \
	} \
	return true; \
}

/** 
Macro que invoca al m�todo que registra la clase en la factor�a.
*/
#define REG_FACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Component_H
