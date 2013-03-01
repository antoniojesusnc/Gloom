//---------------------------------------------------------------------------
// MovableText.h
//---------------------------------------------------------------------------

/**
@file MovableText.h

Contiene la declaraci�n de la clase que permite renderizar texto movible.

@see Graphics::CMovableText

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Graphics_MovableText_H
#define __Graphics_MovableText_H

#include "BaseSubsystems/Math.h"
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreImage.h>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre {
	class Entity;
	class SceneNode;
	class Billboard;
	class BillboardSet;
	class Font;
	class Texture;
}

namespace Graphics {
	class CScene;
}

namespace Graphics {
	
	/**
	
	@ingroup graphicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CMovableText {
	public:

		/**
		Constructor de la clase.

		@param type Tipo del billboard set.
		*/
		CMovableText(const std::string& bbSetName, const Ogre::Vector3& position);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CMovableText();

		/**
		Cambia la posici�n de la entidad.

		@param position Nueva posici�n para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Ogre::Vector3& position);


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
		void setScale(const Ogre::Vector3 &scale);

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

		// A�adir color y fuente
		/**
		@deprecated Los billboards no son buenos para poner texto en pantalla ya que a medida que la distancia
		aumenta se hacen realmente dificiles de leer. La mejor solucion es un overlay que se mueve con la entidad
		*/
		void setText(const std::string& text, const Ogre::Vector3& position, const Ogre::ColourValue& color, 
					 const std::string& fontName = "Verdana", float width = 0, float height = 0);

	protected:

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

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
		bool load();

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
		Ogre::SceneNode* _billboardNode;

		Ogre::BillboardSet* _billboardSet;

		std::list<Ogre::Billboard*> _attachedBillboards;

		Ogre::Vector3 _position;

		/**
		Quiz�s es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene* _scene;

		/**
		Nombre del billboard set. Tiene que ser �nico.
		*/
		std::string _bbSetName;

		/**
		Indica si la entidad ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
