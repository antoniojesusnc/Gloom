/**
@file Entity.h

Contiene la declaraci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"

#include "Logic/Maps/EntityID.h"

// Mensaje
#include "Logic/Messages/Message.h"
#include "Logic/Maps/ComponentID.h"

#include <list>
#include <string>
#include <map>
#include <memory>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
}

// Declaraci�n de la clase
namespace Logic {

	struct ComponentState {
		enum Enum {
			eSLEEPING,
			eAWAKE,
			eBUSY,
			eMAX
		};
	};

	struct TickMode {
		enum Enum {
			eNONE,
			eTICK,
			eFIXED_TICK,
			eBOTH,
			eMAX,
		};
	};

	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cu�les tenga. Adem�s, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creaci�n de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocar� a su m�todo spawn() en el
	que se inicializar�n los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en alg�n momento, cuando se active
	el mapa se llamar� al m�todo activate() de la entidad. En este momento
	los componentes har�n visibles los componentes gr�ficos y dem�s ya que 
	apartir de ah� es cuando la entidad empieza su funci�n siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CEntity {
	public:

		static void setFixedTimeStep(unsigned int stepSize) { _fixedTimeStep = stepSize; }

		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El m�todo llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El m�todo llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funci�n llamada tras la activaci�n de la entidad antes de que se ejecute
		el primer tick. Se garantiza que todas las entidades han sido creadas
		con sus componentes inicializados.

		Esta funci�n SOLO se ejecuta una vez al principio.
		*/
		void start();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		M�todo que a�ade un nuevo componente a la lista de la entidad.

		@param component Componente a a�adir.
		*/
		void addComponent(IComponent* component, const std::string& id);

		/**
		M�todo que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borr� el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		M�todo que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si �ste se especifica en el segundo campo.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviar� el mensaje.
		@return true si al menos un componente acept� el mensaje
		*/
		bool emitMessage(const std::shared_ptr<CMessage>& message, IComponent* emitter = 0);

		/**
		Devuelve el identificador �nico de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }


		void setEntityID(TEntityID id) {_entityID = id; }
		/**
		M�todo que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		dise�o de hacerlo, pero esta es la m�s r�pida: la entidad 
		con la descripci�n de la entidad tiene esta descripci�n que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() { return _isPlayer; }


		/**
		M�todo que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		dise�o de hacerlo, pero esta es la m�s r�pida: la entidad 
		con la descripci�n de la entidad tiene esta descripci�n que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		void setIsPlayer(bool isPlayer){ _isPlayer = isPlayer;}

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde est� la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		void setName(const std::string& name) { this->_name = name; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Establece la matriz de transformaci�n de la entidad. Avisa a los 
		componentes del cambio.

		@param transform Nueva matriz de transformaci�n de la entidad.
		*/
		void setTransform(const Matrix4& transform);

		/**
		Devuelve la metriz de transformaci�n de la entidad.
		<p>
		La posici�n es inicialmente le�da del mapa (si no aparece,
		se colocar� a (0, 0, 0)) y la orientaci�n es tambi�n inicialmente 
		le�da del mapa, como un simple viraje (si no aparece, se colocar� 
		a 0). Obviamente, pueden cambiar con el tiempo.

		@return Matriz de transformaci�n de la entidad en el entorno.
		*/
		Matrix4 getTransform() const { return _transform; }

		/**
		Establece la posici�n de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posici�n.
		*/
		void setPosition(const Vector3 &position);

		/**
		Devuelve la posici�n de la entidad.
		<p>
		La posici�n es inicialmente le�da del mapa (si no aparece,
		se colocar� a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posici�n de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _transform.getTrans(); }

		/**
		Establece la orientaci�n de la entidad. Avisa a los componentes
		del cambio.

		@param pos Nueva orientaci�n.
		*/
		void setOrientation(const Matrix3& orientation);

		/**
		Devuelve la matriz de rotaci�n de la entidad.
		<p>
		La orientaci�n es inicialmente le�da del mapa como un simple 
		viraje (si no aparece, se colocar� a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientaci�n en el entorno.
		*/
		Matrix3 getOrientation() const;

		/**
		Establece el viraje de la entidad. Avisa a los componentes
		del cambio.

		@param yaw Nuevo viraje.
		*/
		void setYaw(float yaw);

		/**
		Vira la entidad. Avisa a los componentes del cambio.

		@param yaw Viraje a aplicar.
		*/
		void yaw(float yaw);

		/**
		Devuelve el viraje de la entidad.
		<p>
		La orientaci�n es inicialmente le�da del mapa como un simple 
		viraje (si no aparece, se colocar� a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Viraje en el entorno.
		*/
		float getYaw() const { return Math::getYaw(_transform); }

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad est� activa.
		*/

		//////////////////

		/**
		Establece el subviraje de la entidad. Avisa a los componentes
		del cambio.

		@param yaw Nuevo viraje.
		*/
		void setPitch(float pitch);

		/**
		Gira verticalmente la entidad. Avisa a los componentes del cambio.

		@param pitch subviraje a aplicar.
		*/
		void pitch(float pitch);

		void roll(float roll);

		//__________________________________________________________________________

		/**
		Devuelve el subviraje de la entidad.
		<p>
		La orientaci�n es inicialmente le�da del mapa como un simple 
		subviraje (si no aparece, se colocar� a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Subviraje en el entorno.
		*/
		float getPitch() const { return Math::getPitch(_transform); }

		//__________________________________________________________________________

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad est� activa.
		*/
		inline bool isActivated() { return _activated; }

		//__________________________________________________________________________

		/**
		 * Obtiene un puntero al componente que buscamos por id.
		 * 
		 * @param id String que identifica al componente. Es el mismo nombre
		 * que el asignado en el blueprints.
		 * @return Puntero al componente que buscamos.
		 */

		template<typename T>
		T* getComponent(const std::string id) {
			std::map<std::string, IComponent*>::iterator it;
			it = _components.find(id);
			if(it == _components.end()) {
				return NULL;
			}

			return static_cast<T*>(it->second);
		}
		//__________________________________________________________________________

		
		/**
		 * Desactiva todos los componentes excepto el paso por par�metro como id.
		 * 
		 * @param id String que identifica al componente. Es el mismo nombre
		 * que el asignado en el blueprints.
		 */
		void deactivateAllComponentsExcept(std::vector<std::string> exceptionList);

		bool dynamicSpawn(CMap* map, Map::CEntity* entityInfo);

	protected:

		static unsigned int _fixedTimeStep;

		unsigned int _acumTime;

		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factor�a CEntityFactory). El constructor
		no hace m�s que poner el identificador �nico de la entidad
		, pues la inicializaci�n efectiva se hace en el m�todo spawn().
		
		@param entityID Identificador �nico de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un m�todo protegido pues para
		eliminar CEntity debe utilizarse la factor�a
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, �ste _ya ha sido desenganchado
		del mapa al que perteneci�_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicializaci�n del objeto Logic, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Map::CEntity). Avisar� a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrar� la entidad.
		@param entity Informaci�n de construcci�n de la entidad le�da del
		fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador �nico de la entidad.
		*/
		Logic::TEntityID _entityID;

		//typedef std::map<CComponentID, IComponent*> IComponentMap;
		typedef std::map<std::string, IComponent*> TComponentMap;

		/**
		 * Hash para indexar a los componentes que nos interesen.
		 * OJO! Nunca usar esta estructura para recorrer secuencialmente
		 * los componentes, para eso tenemos la lista de componentes (que
		 * es mucho m�s eficiente en el recorrido secuencial).
		 */
		TComponentMap _components;

		typedef std::list<IComponent*> TComponentList;

		/** 
		 * @Deprecated
		 *
		 * Lista de componentes usada para ejecutar los ticks. Lo ideal ser�a
		 * utilizar una lista de prioridades (como por ejemplo un Fibonacci Heap)
		 * para que se llame por orden a los ticks de los componentes.
		 * De momento usaremos una lista teniendo cuidado de que los componentes
		 * est�n en el orden adecuado.
		 */
		TComponentList _componentList;

		void (CEntity::*entityProcessMode[ComponentState::eMAX][TickMode::eMAX])(IComponent*, unsigned int, unsigned int);

		/**
		Indica si la entidad est� activa.
		*/
		bool _activated;

		/**
		Tipo de la entidad declarado en el archivo blueprints.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Mapa l�gico donde est� la entidad.
		*/
		Logic::CMap *_map;

		/**
		Matriz de transformaci�n de la entidad. Contiene posici�n y orientaci�n.
		*/
		Matrix4 _transform;

		/*
		Posici�n de la entidad.
		*
		Vector3 _position;

		/*
		Posici�n de la entidad.
		*
		float _orientation;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;


	private:

		void initFunctionPointers();


		void sleepingWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void sleepingWithTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void sleepingWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void sleepingWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps);



		void awakeWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void awakeWithTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void awakeWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void awakeWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps);



		void busyWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void busyWithTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void busyWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps);

		void busyWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps);

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
