/**
@file Light.cpp

Contiene la implementaci�n del componente que controla la vida de una entidad.
 
@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#include "Light.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Light.h"
#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include <OgreSceneManager.h>
#include "Logic/Server.h"

#include <OgreMeshManager.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>
#include <OgreEntity.h>

using namespace Ogre;

namespace Logic {

	IMP_FACTORY(CLight);

	CLight::CLight() {
		// Nada que hacer
	}

	//---------------------------------------------------------

	CLight::~CLight() {

	}
	
	//---------------------------------------------------------
	
	bool CLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Nos aseguramos de que haya una posicion que asignar a la luz
		assert( entityInfo->hasAttribute("position") );

		_light.createPointLight(Graphics::CServer::getSingletonPtr()->getActiveScene(), "luzTest", entityInfo->getVector3Attribute("position") );

		//_light.setposition( entityInfo->getVector3Attribute("position") );

		if( entityInfo->hasAttribute("color") ) {
			Ogre::Vector3 color = entityInfo->getVector3Attribute("color");
			_light.setColour(color.x, color.y, color.z);
		}

		if( entityInfo->hasAttribute("attenuation") ) {
			Ogre::Vector3 attenuation = entityInfo->getVector3Attribute("attenuation");
			// Como de momento no contemplamos el rango de alcance en las luces
			// lo fijamos a un valor arbitrario
			_light.setAttenuation(50.0f, attenuation.x, attenuation.y, attenuation.z);
		}

		/*Vector3 direction;
		
		if(entityInfo->hasAttribute("direction"))
			direction = entityInfo->getVector3Attribute("direction");

		std::string type = entityInfo->getStringAttribute("type");
		if(type == "SpotLight"){
			_light.createSpotlLight(_entity->getMap()->getScene(), _entity->getName(), position, direction);
			if(entityInfo->hasAttribute("innerAngle") && entityInfo->hasAttribute("outerAngle"))
				_light->setRange(entityInfo->getFloatAttribute("innerAngle"), entityInfo->getFloatAttribute("outerAngle") );
		}
		if(type == "DirectionalLight")
			_light->createDirectionalLight(_entity->getMap()->getScene(), _entity->getName(), position, direction);
		if(type == "PointLight")
			_light->createPointLight(_entity->getMap()->getScene(),_entity->getName(), position);

		if(entityInfo->hasAttribute("castShadows"))
			_light->setCastShadows(entityInfo->getBoolAttribute("castShadows"));

		if(entityInfo->hasAttribute("colour")){
			Vector3 colour = entityInfo->getVector3Attribute("colour");
			_light->setColour(colour.x, colour.y, colour.z);
		}
		
		if(entityInfo->hasAttribute("specularColour")){
			Vector3 specularColour = entityInfo->getVector3Attribute("specularColour");
			_light->setSpecularColour(specularColour.x, specularColour.y, specularColour.z);
		}

		if(entityInfo->hasAttribute("intensity"))
			_light->setIntensity(entityInfo->getFloatAttribute("intensity"));
		*/

		//_light->attachToScene();

		return true;

	} // spawn

} // namespace Logic


