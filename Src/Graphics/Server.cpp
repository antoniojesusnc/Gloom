//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementaci�n de la clase principal de gr�ficos, la que permite crear
la ventana, etc.

@see Graphics::CServer

@author David Llans�
@date Julio, 2010
*/

#include "Server.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

namespace Graphics 
{
	CServer *CServer::_instance = 0;

	CServer::CServer() : _root(0), _renderWindow(0), _activeScene(0), _dummyScene(0)
	{
		assert(!_instance && "Segunda inicializaci�n de Graphics::CServer no permitida!");

		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicializaci�n de Graphics::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();

		_renderWindow = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();

		// Creamos la escena dummy para cuando no hay ninguna activa.
		_dummyScene = createScene("dummy_scene");
		
		// Por defecto la escena activa es la dummy
		setScene(_dummyScene);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		if(_activeScene)
		{
			_activeScene->deactivate();
			_activeScene = 0;
		}
		while(!_scenes.empty())
		{
			removeScene(_scenes.begin());
		}

	} // close

	//--------------------------------------------------------
		
	typedef std::pair<std::string,CScene*> TStringScenePar;

	CScene* CServer::createScene(const std::string& name)
	{
		//Nos aseguramos de que no exista ya una escena con este nombre.
		assert(_scenes.find(name)==_scenes.end() && 
			"Ya se ha creado una escena con este nombre.");

		CScene *scene = new CScene(name);
		TStringScenePar ssp(name,scene);
		_scenes.insert(ssp);
		return scene;

	} // createScene

	//--------------------------------------------------------

	void CServer::removeScene(CScene* scene)
	{
		// Si borramos la escena activa tenemos que quitarla.
		if(_activeScene == scene)
			_activeScene = 0;
		_scenes.erase(scene->getName());
		delete scene;

	} // removeScene

	//--------------------------------------------------------

	void CServer::removeScene(const std::string& name)
	{
		CScene* scene = (*_scenes.find(name)).second;
		removeScene(scene);

	} // removeScene

	//--------------------------------------------------------

	void CServer::removeScene(TScenes::const_iterator iterator)
	{
		CScene* scene = (*iterator).second;
		// Si borramos la escena activa tenemos que quitarla.
		if(_activeScene == scene)
			_activeScene = 0;
		_scenes.erase(iterator);
		delete scene;

	} // removeScene

	//--------------------------------------------------------

	void CServer::setScene(CScene* scene)
	{
		// En caso de que hubiese una escena activa la desactivamos.
		if(_activeScene)
			_activeScene->deactivate();

		if(scene)
		{
			// Sanity check. Nos aseguramos de que la escena pertenezca 
			// al servidor. Aunque nadie m�s puede crear escenas...
			assert((*_scenes.find(scene->getName())).second == scene && 
				"Esta escena no pertenece al servidor");

			Ogre::Light *mLuzDireccional;
			mLuzDireccional = scene->getSceneMgr()->createLight("LuzDireccional");
			mLuzDireccional->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional->setDirection(1, 1, 1);
			mLuzDireccional->setDiffuseColour(1, 0, 0);
			////////////////////////////////////////////////////
			Ogre::Light *mLuzDireccional2;
			mLuzDireccional2 = scene->getSceneMgr()->createLight("LuzDireccional2");
			mLuzDireccional2->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional2->setDirection(1, 0, 0);
			mLuzDireccional2->setDiffuseColour(1, 0, 0);

			Ogre::Light *mLuzDireccional3;
			mLuzDireccional3 = scene->getSceneMgr()->createLight("LuzDireccional3");
			mLuzDireccional3->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional3->setDirection(0, 1, 0);
			mLuzDireccional3->setDiffuseColour(1, 0, 0);

			Ogre::Light *mLuzDireccional4;
			mLuzDireccional4 = scene->getSceneMgr()->createLight("LuzDireccional4");
			mLuzDireccional4->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional4->setDirection(0, 0, 1);
			mLuzDireccional4->setDiffuseColour(1, 0, 0);
			//////////////////////////////////////////////////////////////////////
			Ogre::Light *mLuzDireccional5;
			mLuzDireccional5 = scene->getSceneMgr()->createLight("LuzDireccional5");
			mLuzDireccional5->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional5->setDirection(1, 1, 0);
			mLuzDireccional5->setDiffuseColour(1, 0, 0);

			Ogre::Light *mLuzDireccional6;
			mLuzDireccional6 = scene->getSceneMgr()->createLight("LuzDireccional6");
			mLuzDireccional6->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional6->setDirection(1, 0, 1);
			mLuzDireccional6->setDiffuseColour(1, 0, 0);

			///////////////////////////////////////////////////////////////////////
			
			Ogre::Light *mLuzDireccional8;
			mLuzDireccional8 = scene->getSceneMgr()->createLight("LuzDireccional8");
			mLuzDireccional8->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional8->setDirection(0, 1, 1);
			mLuzDireccional8->setDiffuseColour(1, 1, 0);

		/*	Ogre::Light *mLuzDireccional9;
			mLuzDireccional9 = scene->getSceneMgr()->createLight("LuzDireccional9");
			mLuzDireccional9->setType(Ogre::Light::LT_DIRECTIONAL);
			mLuzDireccional9->setDirection(0, 0, 0);
			mLuzDireccional9->setDiffuseColour(0, 255, 0);
			*/
			//NOTA:mLuzPuntual->setPosition(0, 0, 0); eso para la posici�n si es que vale de algo
			//P.D: la luz ambiente ni funciona y hay otras que tampoco, esta es la unica que he conseguido que se note
			//P.D2: si descomentas la luz direccional 9 el suelo se colorea pero tambien todo con un azul tochisimo que se ve incluso peor aun coloreado

			_activeScene = scene;
		}
		// Si se a�ade NULL ponemos la escena dummy.
		else
			_activeScene = _dummyScene;

		_activeScene->activate();

	} // createScene

	//--------------------------------------------------------

	void CServer::setScene(const std::string& name)
	{
		// En caso de que hubiese una escena activa la desactivamos.
		if(_activeScene)
			_activeScene->deactivate();

		// Nos aseguramos de que exista una escena con este nombre.
		assert(_scenes.find(name) != _scenes.end() && 
			"Ya se ha creado una escena con este nombre.");
		_activeScene = (*_scenes.find(name)).second;

		_activeScene->activate();

	} // createScene

	//--------------------------------------------------------

	void CServer::tick(float secs) 
	{
		if(_activeScene != _dummyScene)
			_activeScene->tick(secs);
		if(_root)
		{
			// Actualizamos todas las ventanas de reenderizado.
			Ogre::WindowEventUtilities::messagePump();
			// Reenderizamos un frame
			_root->renderOneFrame(secs);
		}
	} // tick

} // namespace Graphics
