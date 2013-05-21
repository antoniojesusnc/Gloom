#include "Logic/Maps/EntityID.h"
#include "Net/Buffer.h"

#include <map>

namespace Logic{
	//forward declarations
	class CMessage;
	class CEntity;

/**
Clase que guarda todas las entidades que han sido creadas en tiempo de 
ejecuci�n y que para cada una de ellas almacena en qu� estado se encuentra.
Tambi�n almacena la informaci�n de aquellas entidades que han sido 
modificadas y cuya informaci�n es relevante para el otro lado de la red.
<p>
Es una clase singleton que contiene una estructura de datos que almacena
las entidades creadas/modificadas y un pool con el �ltimo mensaje de cada
tipo relevante a ser enviado por la red.
</p><p>
Cuando una entidad se crea din�micamente, la factor�a ha de llamar a esta
clase para introducir los cambios, as� como cuando se destruye una entidad.
Cuando se produzca un cambio relevante, el componente correspondiente debe
comunicarse con esta clase para introducir el cambio que se ha producido.
</p>

*/
	class CWorldState{
	public:

		/**
		Estructura donde guardamos la informaci�n de una entidad relevante
		para el otro lado de la red
		*/
		struct EntityInfo{
			Logic::TEntityID id;
			CEntity* entity;
			std::map<unsigned int, std::shared_ptr<CMessage>> messages;
		};

		// =======================================================================
		//                          METODOS EST�TICOS
		// =======================================================================

		CWorldState();

		~CWorldState(){_instance = 0;}

		/**
		Devuelve la �nica instancia de la clase CWorldState.
		
		@return �nica instancia de la clase CWorldState.
		*/
		static CWorldState* getSingletonPtr(){return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CWorldState. Debe llamarse en
		la destrucci�n de las estructuras de la l�gica
		*/
		static void Release();

		// =======================================================================
		//                          METODOS P�BLICOS
		// =======================================================================

		void addEntity(CEntity* entity);

		void deleteEntity(CEntity* entity);

		void addChange(CEntity* entity, std::shared_ptr<CMessage> message);

		void clearEntities();

		/**
		M�todo que serializa la informaci�n contenida en el estado del mundo y la deja
		preparada en un buffer para enviarla por la red.
		Para poder deserializar la informaci�n, este buffer debe pasarse al m�todo
		deserialize desde el otro lado de la red, ya que esta clase es la que sabe
		c�mo la informaci�n es almacenada en el buffer.

		@return El buffer con toda la informaci�n del estado del mundo lista para 
		enviar por la red.
		*/
		Net::CBuffer serialize();

		/**
		M�todo que dado un buffer deserializa toda la informaci�n y la carga en el
		mundo para que est� actualizado con el servidor.
		Para que este m�todo funcione correctamente, el buffer debe haber sido 
		serializado por el m�todo deserialize de esta clase desde el otro lado de la 
		red.

		@param worldState Buffer que contiene toda la informaci�n del estado del
		mundo.
		*/
		void deserialize(Net::CBuffer &worldState);

		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================

	protected:


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

	private:

		// =======================================================================
		//                          MIEMBROS DE CLASE
		// =======================================================================
		/**
		�nica instancia de la clase.
		*/
		static CWorldState* _instance;

		/**
		Tabla hash donde almacenamos que players han sido creados/modificados
		*/
		std::map<TEntityID,EntityInfo> _entities;

		/**
		Comfort typedefs
		*/
		typedef std::pair<TEntityID, EntityInfo> TEntityInfo;
		typedef std::pair<unsigned int, std::shared_ptr<CMessage>> TInfo;
	};

}