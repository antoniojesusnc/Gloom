/**
@file Map.h

Contiene la declaraci�n de la clase CMap, Un mapa l�gico.

@see Logic::Map

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_Map_H
#define __Logic_Map_H

#include <map>
#include <set>
#include <list>
#include <unordered_map>
#include "EntityID.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
}

namespace Map
{
	class CEntity;
}

namespace Graphics 
{
	class CScene;
}

// Declaraci�n de la clase
namespace Logic
{
	/**
	Clase que representa un mapa l�gico.
	<p>
	Esta clase se encarga de almacenar todas las entidades del mapa, tanto 
	aquellas que tienen representaci�n gr�fica y se ven como entidades
	puramente l�gicas. Gestiona la activaci�n y desactivaci�n de �stas y
	tiene tambi�n m�todos para buscar entidades, tanto por su nombre como 
	por su tipo y por su identificador.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CMap {
	public:

		/**
		M�todo factor�a que carga un mapa de fichero. Tras el parseo de
		todas las entidades del mapa mediante CMapParser, genera todas las
		entidades con CEntityFactory.

		@param filename Nombre del archivo a cargar.
		@return Mapa generado.
		*/
		static CMap *createMapFromFile(const std::string &filename);

		/**
		Constructor.

		@param name Nombre que se le da a este mapa.
		*/
		CMap (const std::string &name);

		/**
		Destructor.
		*/
		~CMap();
		
		/**
		Activa el mapa. Invocar� al m�todo activate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true al invocador si todas las entidades se
		activaron sin problemas.
		*/
		bool activate();

		/**
		Desactiva el mapa. Invocar� al m�todo deactivate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funci�n llamada tras la carga del mapa antes de que se ejecute
		el primer tick.
		*/
		void start();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todas las entidades.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		A�ade una nueva entidad al mapa. Si la entidad ya estaba incluida
		no se hace nada.

		@param entity Entidad a a�adir.
		*/
		void addEntity(CEntity *entity);

		/**
		Elimina una entidad del mapa. Si la entidad no estaba incluida
		no se hace nada. La funci�n desactiva previamente la entidad si
		�sta se encontraba activa.

		@note El mapa no se hace responsable de eliminar (con delete) la
		entidad.

		@param entity Entidad a eliminar.
		*/
		void removeEntity(CEntity *entity);

		/**
		Elimina y destruye todas las entidades del mapa dejando la lista 
		de entidades vac�a.
		*/
		void destroyAllEntities();

		/**
		Recupera una entidad del mapa a partir de su ID.

		@param entityID ID de la entidad a recuperar.
		@return Entidad con el ID pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByID(TEntityID entityID);

		/**
		Recupera una entidad del mapa a partir de su nombre.

		@param name nombre de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. �til si se tienen varias entidades con el mismo
		nombre y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByName(const std::string &name);

		/**
		Recupera una entidad del mapa a partir de su tipo.

		@param type nombre del tipo de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. �til si se tienen varias entidades del mismo tipo
		y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		std::list<CEntity*> getEntitiesByType(const std::string &type);

		/**
		Devuelve la escena gr�fica correspondiente a este mapa.

		@return Escena con las entidades gr�ficas.
		*/
		Graphics::CScene *getScene() {
			return _scene;}
		
		std::string getMapName(){return _name;}

		/**
		Crea un nuevo jugador y le porporciona un nombre determinado.

		@param name Nombre del jugador.
		*/

		CEntity* createPlayer(const std::string &name, const std::string &type);

		/**
		Crea un nuevo jugador y le porporciona un nombre determinado.

		@param name Nombre del jugador.
		@param id La id de la entidad que devuelve el m�todo.
		*/
		CEntity* createLocalPlayer(const std::string &name, const std::string &type, TEntityID id);

		CEntity* createPlayer(const std::string &name, const std::string &type, TEntityID id);

		/**
			M�todo que elimina una entidad en el siguiente tick del mapa
			@param entity Entidad que ser� borrada
		*/
		void deleteDeferredEntity(CEntity* entity);

		void entityTimeToLive(CEntity* entity, unsigned int msecs);

		void setFixedTimeStep(unsigned int stepSize);

		void wantsTick(CEntity* entity);

		void wantsFixedTick(CEntity* entity);

	private:

		void checkTimeouts(unsigned int msecs);

		void processComponentMessages();

		void doTick(unsigned int msecs);

		void doFixedTick(unsigned int msecs);

		/**
		Lista de entidades que han sido marcadas para ser borradas en
		un tiempo dado.
		*/
		std::list< std::pair<CEntity*, unsigned int> > _entitiesWithTimeout;

		struct EntityInfo {
			CEntity* _entityPtr;
			std::list<CEntity*>::const_iterator _processIterator;
			std::list<CEntity*>::const_iterator _tickIterator;
			std::list<CEntity*>::const_iterator _fixedTickIterator;
		};

		//std::unordered_map<TEntityID, EntityInfo> _entityInfoTable;

		// Hash de punteros a informacion de entidades
		std::unordered_map<TEntityID, EntityInfo*> _entityIdLookupTable;
		std::unordered_map<std::string, EntityInfo*> _entityNameLookupTable;
		std::unordered_multimap<std::string, EntityInfo*> _entityTypeLookupTable;
		
		// Listas de entidades a recorrer
		std::list<CEntity*> _entityList;
		std::list<CEntity*> _entitiesWithTick;
		std::list<CEntity*> _entitiesWithFixedTick;

		/**
		Tipo tabla de entidades de mapa.
		*/
		//typedef std::map<TEntityID, CEntity*> TEntityMap;

		/**
		tabla con las entidades del mapa localizadas por su ID.
		*/
		//TEntityMap _entityMap;

		/**
		Lista de entidades que hay que borrar
		*/
		std::list<CEntity*> _deleteEntities;

		/**
		Nombre del mapa.
		*/
		std::string _name;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades.
		*/
		Graphics::CScene* _scene;

		/**
		@deprectaed -> no se usa en ninguna parte.
		N�mero de jugadores creados hasta el momento.
		*/
		unsigned int _numOfPlayers;

		/**
		Variable de clase que indica el n�mero de milisegundos que se procesan en cada
		iteraci�n del tick fijo.
		*/
		unsigned int _fixedTimeStep;

		/** Tiempo acumulado a tener en cuenta para el tick fijo. */
		unsigned int _acumTime;

	}; // class CMap

} // namespace Logic

#endif // __Logic_Map_H
