//---------------------------------------------------------------------------
// Light.h
//---------------------------------------------------------------------------

/**
@file Light.h

Contiene la declaraci�n de la clase que representa una Luz.

@see Graphics::Clight

@author Rub�n Mulero
@date March, 2013
*/

#ifndef __Graphics_Light_H
#define __Graphics_Light_H

#include "BaseSubsystems/Math.h"



// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class SceneNode;
	class Light;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{
	/**
	Clase que representa las luces en el escenario de ogre, contiene una referencia
	que representa la luz de ogre y otra que es el nodo de la escena al que est� 
	asociada esa luz
	Al construir la clase no se crean las luces, la construcci�n de un objeto de
	esta clase representa la luz, pero hay que usar los m�todos que nos proporciona
	para crear y manejar la clase a nuestro antojo
	*/
	class CLight{
	public:
		CLight(): _node(NULL), _light(NULL) {}
		~CLight();
		/**
		M�todo que crea una luz direccional en la posici�n dada, que mira en la
		direcci�n que se le indica como par�metro
		@param name El nombre de la luz que se va a crear
		@param position la posici�n de la escena donde se va a crear la luz
		@param direction Direcci�n en la que mira la luz que se crea
		@return true si todo fue bien, false en caso contrario
		*/
		bool createDirectionalLight(std::string name, Vector3 position, Vector3 direction);
		/**
		M�todo que crea una luz puntual en la posici�n dada, que mira en la
		direcci�n que se le indica como par�metro
		@param name El nombre de la luz que se va a crear
		@param position la posici�n de la escena donde se va a crear la luz
		@param direction Direcci�n en la que mira la luz que se crea
		@return true si todo fue bien, false en caso contrario
		*/
		bool createPointLight(std::string name, Vector3 position);
		/**
		M�todo que crea una luz local en la posici�n dada.
		@param name El nombre de la luz que se va a crear
		@param position la posici�n de la escena donde se va a crear la luz
		@return true si todo fue bien, false en caso contrario
		*/
		bool createSpotlLight(std::string name, Vector3 position, Vector3 direction);

		/**
		M�todo que destruye la luz y la elimina de la escena
		*/
		bool destroyLight();
		/**
		M�todo controla las sombras que castea la luz.
		@param enabled indica si las sombras se activan o se desactivan
		*/
		void setCastShadows(bool enabled);
		/**
		M�todo que setea el color de la luz
		@param r el rojo
		@param g el verde
		@param b el azul
		*/
		void setColour(float r, float g, float b);
		/**
		M�todo que setea el color difuso de la luz
		@param r el rojo
		@param g el verde
		@param b el azul
		*/
		void setSpecularColour(float r, float g, float b);

		/**
		M�todo que devuelve la direcci�n de la luz
		@return la direcci�n en la que mira la luz
		*/
		Vector3 getDirection(){return _light->getDirection();}
		/**
		M�todo que setea la direcci�n de la luz
		@param direction la direcci�n en la que queremos que mire la luz
		*/
		void setDirection(Vector3 direction){_light->setDirection(direction);}
		/**
		M�todo que devuelve la posici�n de la luz
		@return la posici�n de la luz
		*/
		Vector3 getPosition(){return _light->getDirection();}
		/**
		M�todo que setea la posici�n de la luz
		@param position la posici�n en la que queremos la luz
		*/
		void setposition(Vector3 position){_light->setPosition(position);}

		/**
		M�todo que setea la fuerza de la luz
		@param intensiy la fuerza emisora de la luz.
		*/
		void setIntensity(float intensiy){_light->setPowerScale(intensiy);}

		/**
		M�todo que mete la luz en la escena de ogre para que se pueda visualizar en el mapa
		*/
		void attachToScene();

	

	protected:
		Ogre::Light* _light;

		Ogre::SceneNode * _node;
	};

}

#endif