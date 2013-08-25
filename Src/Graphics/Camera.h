//---------------------------------------------------------------------------
// Camera.h
//---------------------------------------------------------------------------

/**
@file Camera.h

Contiene la declaraci�n de la clase que maneja la c�mara.

@see Graphics::CCamera

@author Jose Antonio Garc�a Y��ez
@date Mayo, 2013
*/

#ifndef __Graphics_Camera_H
#define __Graphics_Camera_H

#include "BaseSubsystems/Math.h"
#include <OgreCamera.h>
#include <list>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Camera;
	class SceneNode;
}
namespace Graphics 
{
	class CScene;
	class CServer;
	class CEntity;
}
namespace Logic
{
	class CCamera;
}

namespace Graphics 
{
	/**
	Clase de la c�mara extendida basada en Ogre.
	La c�mara est� compuesta por un �nico nodo.
	
	@ingroup graphicsGroup

	@author Jose Antonio Garc�a Y��ez
	@date Mayo, 2013
	*/
	class CCamera
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la c�mara.
		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CCamera(const std::string &name, CScene *scene);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CCamera();
		

		/**
		Devuelve la posici�n de la c�mara.

		@return Referencia a la posici�n del nodo que contiene la c�mara de Ogre.
		*/
		const Vector3 &getCameraPosition();
		

		/**
		Devuelve la orientaci�n de la c�mara.

		@return Referencia al quaternion del nodo que contiene la c�mara de Ogre.
		*/
		const Quaternion &getCameraOrientation();


		/**
		Cambia la posici�n de la c�mara.

		@param newPosition Nueva posici�n para el nodo que contiene la c�mara 
		de Ogre.
		*/
		void setCameraPosition(const Vector3 &newPosition);


		/**
			get & set de CameraDirection
		*/
		void setCameraDirection(const Vector3& direction);
		const Vector3 &getCameraDirection();


		/**
		Devuelve el quaternion en el mundo real de la c�mara.
		*/
		const Quaternion& getRealOrientation ();


		/**
		Roll de camera. Gira la camara en el sentido contrario
		a las agujas del reloj.

		El giro comienza a partir de la z local (o el norte):
		- Un giro de 2pi deja a la camara sin rotar, un giro
		de pi dejaria la camara boca abajo.

		@param Radianes de giro de la camara.
		*/
		void rollCamera(float fRadian);


		/**
		Devuelve un puntero a la camara creada por Ogre.

		@return Puntero a la camara creada por Ogre.
		*/
		Ogre::Camera* getOgreCamera() { return _camera; }	


		/**
		Metodo que crea una entidad grafica asociada a la camara, de tal modo que la posicion sera relativa a la camara.
		�IMPORTANTE! esto crea una entidad, el destructor de la camara no elimina dicha entidad, se debera de eliminar ella por separado

		@param nameEntity, nombre de la entidad que se creara
		@param nameMesh, nombre de la maya que se creara.
		@param position, Vector3 con la posicion 
		@return CEntity* devuelve un puntero a una entidad grafica. 
		*/
		Graphics::CEntity* CCamera::addEntityChild(const std::string &nameEntity, const std::string &nameMesh, Vector3 position = Vector3::ZERO);


		/**
		Establece la orientacion de la camara utilizando el quaternion pasado como par�metro.

		@param orientation Orientacion puesta en quaternion para setear.
		*/

		void setOrientation(const Quaternion& orientation);


		/**
		M�todo que cambia el FOV de la c�mara por uno especificado por parametro
		*/
		void setFov(float radianAngle);

		/**
		Establece la orientacion para mirar al punto pasado por parametro.

		@param position Posicion hacia donde mirar� la camara.
		*/
		void lookAt(const Vector3& position);

		/**
		M�todo que resetea el fov y lo pone por defecto
		*/
		void resetFov();

		Ogre::SceneNode* getSceneNode() { return _cameraNode; }

	protected:
		
		/**
		Clase amiga. Solo la escena tiene acceso a la c�mara de Ogre para
		poder crear el viewport.
		*/
		friend class CScene;

		/**
		Devuelve la c�mara de Ogre.

		@return puntero a la c�mara de Ogre.
		*/
		Ogre::Camera *getCamera() {return _camera;}

		/**
		Nodos que contienen la c�mara.
		*/
		Ogre::SceneNode *_cameraNode;
		
		/**
		La c�mara de Ogre.
		*/
		Ogre::Camera *_camera;

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la c�mara.
		*/
		std::string _name;

		float _defaultFov;

	}; // class CCamera

} // namespace Graphics

#endif // __Graphics_Camera_H
