//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CEntity

@author
@date 
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Entity;
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
	class CObjectTextDisplay;
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gr�fica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escenada de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al a�adir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gr�fica (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas. Por ejemplo, �sta clase
	no permite especificar la animaci�n utilizada. Si queremos que podamos
	guardar un modelo gr�fico, y poder cambiar la animaci�n actual,
	se utilizar� la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author 
	@date 
	*/
	class CEntity 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CEntity();

		/**
		Cambia la posici�n y orientaci�n de la entidad.

		@param position Vector de posicion a setear en la entidad.
		@param orientation Orientacion en quaternion a setear en la entidad.
		*/
		void setTransform(const Vector3 &position, const Ogre::Quaternion &orientation);

		/**
		Cambia la posici�n de la entidad.

		@param position Nueva posici�n para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientaci�n de la entidad.

		@param orientation Referencia al quaternion de rotaci�n con el que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Quaternion &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad est� puesta invisible, no deber�a dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		Setea la entidad de ogre en nuestra referencia.

		@param scene Referencia a la entidad de Ogre.
		*/
		void setOgreEntity(Ogre::Entity* entity);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		 Escala la entidad.

		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala la entidad.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/**
		 Devuelve el valor de la escala de la entidad.

		 @return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		/**
		Devuelve el transform de la entidad

		@return Matrix4 del la entidad
		*/
		Matrix4 getTransform();

		/**
		Devuelve la entidad de Ogre.

		@return puntero a la entidad de Ogre.
		*/
		Ogre::Entity* getEntity() {return _entity;}

		/**
		Devuelve el nombre de la entidad de Ogre.

		@return nombre de la entidad en string.
		*/
		std::string getName(){return _name;}

		/**
		Devuelve el nombre de la malla de la entidad.

		@return nombre de la malla de la entidad en string.
		*/
		std::string getMesh(){return _mesh;}

		/**
		Devuelve el nodo que contiene la entidad en Ogre.

		@return puntero al nodo que contiene la entidad.
		*/
		Ogre::SceneNode* getSceneNode() {return _entityNode;}

		/**
		Setea el nodo de ogre en nuestra referencia y pone el atributo de carga a verdadero.

		@param sceneNode Referencia al nodo de Ogre.
		*/
		void setSceneNode(Ogre::SceneNode *sceneNode) {_entityNode = sceneNode;_loaded = true;}

		/**
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Setea la escena de ogre en nuestra referencia.

		@param scene Referencia a la escena de Ogre.
		*/
		void setScene(CScene *scene){_scene=scene;}

		/**
		Metodo que cambia el material de una entidad.

		@param materialName Nombre del material al que queremos cambiar.
		*/
		virtual void changeMaterial(const std::string& materialName);


		/**
		Metodo que obtiene toda la informaci�n de la malla cargada en la entidad.
		*/
		void getMeshInformation(const Ogre::MeshPtr& mesh,size_t &vertex_count, Ogre::Vector3* &vertices,
								size_t &index_count, unsigned* &indices,
								const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
								const Ogre::Quaternion &orient = Ogre::Quaternion::IDENTITY,
								const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);

		/**
		Metodo que devuelve un puntero a la escena que contiene la entidad.

		@return puntero a la escena que contiene la entidad.
		*/
		CScene*			getScene			() { return _scene; }

	protected:

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y elimin� de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode *_entityNode;

		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;

		/**
		Quiz�s es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;

		/**
		Indica si la entidad ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;

		//CObjectTextDisplay* _text;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
