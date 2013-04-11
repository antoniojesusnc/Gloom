﻿/**
@file Life.cpp

Contiene la implementaci�n del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llans�
@date Octubre, 2010
*/

#include "HudOverlay.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"


#include "Graphics/Particle.h"

#include "Graphics/Server.h"
#include "Graphics/Camera.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/Scene.h"
#include "Graphics/Overlay.h"
#include "Graphics/StaticEntity.h"

/*
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>
#include <OgreOverlayContainer.h>
#include <OgreColourValue.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreEntity.h>
*/




#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudSpawn.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageHudDebug.h"
#include "Logic/Messages/MessageHudDebugData.h"

namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	CHudOverlay::~CHudOverlay(){
		
		if(_overlayPlay)
			delete _overlayPlay;
		if(_overlayDie)
			delete _overlayDie;
		if(_textAreaDie)
			delete _textAreaDie;
		
		for(int i=0;i<3;++i){
		
			if(_panelElements[i])
				delete _panelElements[i];

			if(_panelElementsTextArea)
				delete _panelElementsTextArea[i];

		}

		if(_panelMira)
			delete _panelMira;

		for(int i=0;i<6;++i){
			if(_panelWeapon[i])
				delete _panelWeapon[i];
			
			if(_panelWeaponKey[i]){
				delete _panelWeaponKey[i];	
			}

			if(_overlayWeapon3D[i])
				delete _overlayWeapon3D[i];
			//Graphics::CServer::getSingletonPtr()->getActiveScene()->removeEntity( _weaponsEntities[i]);

			if(_weaponsEntities[i])
				_weaponsEntities[i] = 0;
			
			for(int j=0;j<3;++j){
				if(_weaponsBox[i][j])
					delete _weaponsBox[i][j];
			}
		}

		if(_overlayDebug)
			delete _overlayDebug;

		if(_panelDebug)
			delete _panelDebug;

		if(_textAreaDebug)
			delete _textAreaDebug;

	}

	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		char keysAux[8] = {'1','2','3','4','5','6','Q','E'};

		for (int i =0 ; i<8;++i){
			keysPanelWeapon[i] = keysAux[i];
		}
		
		
		//memcpy (keysAux, keysPanelWeapon, sizeof (keysAux)) ;

		Graphics::CScene* scene = map->getScene();
		_numWeapons = entityInfo->getIntAttribute("numWeapons");
	
		_server = Graphics::CServer::getSingletonPtr();


		int hudScreenWidth = entityInfo->getIntAttribute("hudScreenWidth");
		int hudScreenHeight = entityInfo->getIntAttribute("hudScreenHeight");

		int hudPanelInitialPositionX = entityInfo->getIntAttribute("hudPanelInitialPositionX");
		int hudPanelInitialPositionY = entityInfo->getIntAttribute("hudPanelInitialPositionY");
		int hudPanelSizeX = entityInfo->getIntAttribute("hudPanelSizeX");
		int hudPanelSizeY = entityInfo->getIntAttribute("hudPanelSizeY");

		int height = _server->getHeight();
		int width = _server->getWidth();

		float relativeWidth = ((float)width)/hudScreenWidth;
		float relativeHeight = ((float)height)/hudScreenHeight;

		////////////////Todo esto para la mira
         // Create an overlay
		_overlayPlay = _server->createOverlay( "_overlayPlay", scene );		
         
		// Create a panel de Mira
		_panelMira = _server->createOverlay("Mira", scene, "Panel" );
		float sizeCrossFireX = entityInfo->getFloatAttribute("hudCrossX");
		float sizeCrossFireY = entityInfo->getFloatAttribute("hudCrossY");
		float positionCrossFireX = 0.5f-((sizeCrossFireX/2)*0.01) ;
		float positionCrossFireY = 0.5f-((sizeCrossFireY/2)*0.01) ;
        _panelMira->setPosition( positionCrossFireX,positionCrossFireY);
		_panelMira->setDimensions( sizeCrossFireX/100, sizeCrossFireY/100 );
        _panelMira->setMaterial("hudMira");


		// overlay para controlar las teclas
			Graphics::COverlay *textAreaDummy = _server->createOverlay("dummy", scene, "TextArea");
				
			textAreaDummy->setMetricsMode("pixel");
			textAreaDummy->setPosition(200, 200);
			textAreaDummy->setDimensions(0, 0);
			textAreaDummy->setText("dummy text");

			// hardcodea el tamaño y tipo de la fuente
			textAreaDummy->setTextSize(16);
			textAreaDummy->setFont("fuenteSimple");

			_panelMira->addChild(textAreaDummy);

		_overlayPlay->add2D( _panelMira );


         // Add the panel to the overlay

		int x = hudPanelInitialPositionX;
		int y = hudPanelInitialPositionY;


		for(int i=0; i< PRIMARY_SKILL; ++i){

			eWeaponIndex current = (eWeaponIndex)i;
			std::string currentOnText = toText(current);
		
			// Primero me creo el panel que estaba debajo
			_panelWeapon[current] = _server->createOverlay( "PanelWeapon" + currentOnText, scene, "Panel");
			_panelWeapon[current]->setMetricsMode("pixel");
			_panelWeapon[current]->setPosition( x*relativeWidth, y*relativeHeight);
			_panelWeapon[current]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeX );
			_panelWeapon[current]->setMaterial("cuadroArmas");

			

		
			// overlay para controlar las teclas
			_panelWeaponKey[current] = _server->createOverlay(currentOnText+"_Key", scene, "TextArea");
				
			_panelWeaponKey[current]->setMetricsMode("pixel");
			_panelWeaponKey[current]->setPosition(1*relativeWidth, -1*relativeHeight);
			_panelWeaponKey[current]->setDimensions(relativeWidth, relativeHeight);
				std::stringstream sString;//create a stringstream
				sString << keysPanelWeapon[current];
			_panelWeaponKey[current]->setText(sString.str());

			// hardcodea el tamaño y tipo de la fuente
			_panelWeaponKey[current]->setTextSize(20);
			_panelWeaponKey[current]->setFont("fuenteSimple");

			_panelWeapon[current]->addChild(_panelWeaponKey[current]);
			_panelWeapon[current]->setZBuffer(0);
			
				
			_overlayPlay->add2D( _panelWeapon[current]);

			//Aqui me creo el cuadro de ACTIVE, NO_AMMO, NO_WEAPON
			for(int j=0; j<=NO_WEAPON; ++j){
				eOverlayWeaponState currentState = (eOverlayWeaponState)j;
				std::string currentStateOnText = toText(currentState);
				
				_weaponsBox[current][currentState] = _server->createOverlay( "_weaponsBox["+currentOnText+"]["+currentStateOnText+"]", scene, "Panel");
				_weaponsBox[current][currentState]->setMetricsMode("pixel");
				_weaponsBox[current][currentState]->setPosition( x*relativeWidth, y*relativeHeight);
				_weaponsBox[current][currentState]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeY );
				_weaponsBox[current][currentState]->setMaterial(currentOnText +"_"+ currentStateOnText);
				_weaponsBox[current][currentState]->setVisible(false);

				_overlayPlay->add2D( _weaponsBox[current][currentState] );
			}
			// Ya que todos los he puesto invisibles, el sin arma me lo pongo a visible
			_weaponsBox[current][NO_WEAPON]->setVisible(true);

			// Actualizo la posicion de los paneles
			x += hudPanelSizeX;
			// La posicion y no debe cambiar, ya que todo ira a la misma altura.
			//y += hudPanelSizeY;



			////////////////////////////////////////////////////// Ahora voy a crear los overlays por cada arma en 3D

			_overlayWeapon3D[current] = _server->createOverlay( "_overlay3D"+currentOnText, scene );
			std::string modelWeapon = entityInfo->getStringAttribute("weapon"+currentOnText+"Model");
			Vector3 offsetPositionWeapon = entityInfo->getVector3Attribute("weapon"+currentOnText+"Offset");
			
			
			_weaponsEntities[current] = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon, &offsetPositionWeapon);
			//_weaponsEntities[current] = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon, new Vector3(0,0,-10));
			float yaw = entityInfo->getFloatAttribute("weapon"+currentOnText+"ModelYaw");
			float pitch = entityInfo->getFloatAttribute("weapon"+currentOnText+"ModelPitch");
			Matrix4 transformModificado = _weaponsEntities[current]->getTransform();
			
			Math::yaw(yaw, transformModificado);
			Math::pitch(pitch, transformModificado);
			
			_weaponsEntities[current]->setTransform(transformModificado);

			_overlayWeapon3D[current]->setVisible(false);

		 } // For de gestion de cada arma.


		// Bucle para la gestion de la primary skill.

		x -= hudPanelSizeX*4;
		y -= hudPanelSizeY*1.5;

		for(int i=PRIMARY_SKILL; i< NONE; ++i){

			eWeaponIndex current = (eWeaponIndex)i;
			std::string currentOnText = toText(current);
			// Primero me creo el panel que estaba debajo
			_panelWeapon[current] = _server->createOverlay( "PanelWeapon" + currentOnText, scene, "Panel");
			_panelWeapon[current]->setMetricsMode("pixel");
			_panelWeapon[current]->setPosition( x*relativeWidth, y*relativeHeight);
			_panelWeapon[current]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeX );
			_panelWeapon[current]->setMaterial("cuadroArmas");

			_overlayPlay->add2D( _panelWeapon[current]);

			// overlay para controlar las teclas
			_panelWeaponKey[current] = _server->createOverlay(currentOnText+"_Key", scene, "TextArea");

			_panelWeaponKey[current]->setMetricsMode("pixel");
			_panelWeaponKey[current]->setPosition(1*relativeWidth, -1*relativeHeight);
			_panelWeaponKey[current]->setDimensions(relativeWidth, relativeHeight);
				std::stringstream sString;//create a stringstream
				sString << keysPanelWeapon[current];
			_panelWeaponKey[current]->setText(sString.str());

			// hardcodea el tamaño y tipo de la fuente
			_panelWeaponKey[current]->setTextSize(24);
			_panelWeaponKey[current]->setFont("fuenteSimple");

			_panelWeapon[current]->addChild(_panelWeaponKey[current]);

			//Aqui me creo el cuadro de ACTIVE, NO_AMMO, NO_WEAPON
			for(int j=0; j<=NO_WEAPON; ++j){
				eOverlayWeaponState currentState = (eOverlayWeaponState)j;
				std::string currentStateOnText = toText(currentState);
				
				_weaponsBox[current][currentState] = _server->createOverlay( "_weaponsBox["+currentOnText+"]["+currentStateOnText+"]", scene, "Panel");
				_weaponsBox[current][currentState]->setMetricsMode("pixel");
				_weaponsBox[current][currentState]->setPosition( x*relativeWidth, y*relativeHeight);
				_weaponsBox[current][currentState]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeY );
				_weaponsBox[current][currentState]->setMaterial(currentOnText +"_"+ currentStateOnText);
				_weaponsBox[current][currentState]->setVisible(false);

				_overlayPlay->add2D( _weaponsBox[current][currentState] );
			}
			// Ya que todos los he puesto invisibles, el sin arma me lo pongo a visible
			_weaponsBox[current][NO_WEAPON]->setVisible(true);
		}

		// fin de la introduccion de la primary skill
		
	
		// en el HAMMER (que es el arma inicial, debe de estar active)
		_weaponsBox[HAMMER][ACTIVE]->setVisible(true);
		_weaponsBox[HAMMER][NO_WEAPON]->setVisible(false);
		_overlayWeapon3D[HAMMER]->setVisible(true);
		
		// Ahora me voy a crear otro bucle para los paneles.
		
		float hudPanelpositionX;
		float hudPanelpositionY;
		float hudPanelWidth;
		float hudPanelHeight;
		std::string hudPanelFont;
		int hudPanelFontSize;
		float hudPanelTextPositionXRelative;
		float hudPanelTextPositionYRelative;


		for(int i=0; i<3;++i){
		
			eOverlayElements current = (eOverlayElements)i;
			std::string currentOnText = toText(current);
	
			//Recopilo todos los datos necesarios para añadir los paneles
			hudPanelpositionX = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"PositionX");
			hudPanelpositionY = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"PositionY");
			hudPanelWidth = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"Width");
			hudPanelHeight = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"Height"); 
			hudPanelFont = entityInfo->getStringAttribute("hudPanel"+currentOnText+"Font");
			hudPanelFontSize = entityInfo->getIntAttribute("hudPanel"+currentOnText+"FontSize");
			hudPanelTextPositionXRelative = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"TextPositionXRelative");
			hudPanelTextPositionYRelative = entityInfo->getFloatAttribute("hudPanel"+currentOnText+"TextPositionYRelative");
			
			_panelElements[current] = _server->createOverlay(currentOnText, scene,  "Panel" );
			_panelElements[current]->setMetricsMode("pixel");
			_panelElements[current]->setPosition( hudPanelpositionX*relativeWidth, hudPanelpositionY*relativeHeight);
			_panelElements[current]->setDimensions( relativeWidth * hudPanelWidth, relativeHeight * hudPanelHeight );
			_panelElements[current]->setMaterial("hud_"+currentOnText);

			_panelElementsTextArea[current] = _server->createOverlay(currentOnText+"_TextArea", scene, "TextArea");
				
				_panelElementsTextArea[current]->setMetricsMode("pixel");
				_panelElementsTextArea[current]->setPosition(relativeWidth*hudPanelTextPositionXRelative, hudPanelTextPositionYRelative);
				_panelElementsTextArea[current]->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream sString;//create a stringstream
					sString << _panelElementsText[current];//add number to the stream
				_panelElementsTextArea[current]->setText(sString.str());
				_panelElementsTextArea[current]->setTextSize(hudPanelFontSize);
				_panelElementsTextArea[current]->setFont(hudPanelFont);

				_panelElements[current]->addChild(_panelElementsTextArea[current]);

		_overlayPlay->add2D( _panelElements[current] );

		}

		//Pongo a false los visibles por si acaso no los pone solos
		for(int i = 1; i < _numWeapons; ++i){
			_weaponsBox[i][NO_AMMO]->setVisible(false);
			_weaponsBox[i][ACTIVE]->setVisible(false);
		}
		
		//_overlayPlay->add3D("AK47", "AK47.mesh", new Vector3(0,0,-5));
		
		_overlayPlay->setVisible(true);



		 //////////////////////////////////////AQUI ME CREO EL OVERLAY PARA CUANDO SE MUERA

		_overlayDie = _server->createOverlay( "_overlayDie", scene );

		Graphics::COverlay* panelDie = _server->createOverlay("panelDie", scene, "Panel");
		panelDie->setMetricsMode("pixel");
		panelDie->setPosition( 5*relativeWidth, 12*relativeHeight);
		panelDie->setDimensions( relativeWidth*10, relativeHeight*10 );
        //panelDie->setMaterial("cuadroArmas");

		_textAreaDie = _server->createOverlay("_textAreaDie", scene, "TextArea");

				_textAreaDie->setMetricsMode("pixel");
				_textAreaDie->setPosition(0.5, 0.5);
				_textAreaDie->setDimensions(10, 10);

				_textAreaDie->setText("MUERE");
				_textAreaDie->setTextSize(46);
				_textAreaDie->setFont("fuenteSimple");
				
				panelDie->addChild(_textAreaDie);

		_overlayDie->add2D( panelDie );

		//_overlayDie->show();


		//////////////////////////////////////AQUI ME CREO EL OVERLAY PARA EL DEBBUG

		_overlayDebug = _server->createOverlay( "_overlayDebug", scene );

		_panelDebug = _server->createOverlay("panelDebug", scene, "Panel");
		panelDie->setMetricsMode("pixel");

		//_panelDebug->setPosition(0,0);

		_panelDebug->setPosition( entityInfo->getFloatAttribute("hudPanelDebugPositionX")*relativeWidth, 
			entityInfo->getFloatAttribute("hudPanelDebugPositionY")*relativeHeight);
		
		_panelDebug->setDimensions( entityInfo->getFloatAttribute("hudPanelDebugWidth")*relativeWidth,
			entityInfo->getFloatAttribute("hudPanelDebugHeight")*relativeHeight );
        //_panelDebug->setMaterial("cuadroArmas");
		
		_panelDebug->setVisible(true);

		_textAreaDebug = _server->createOverlay("_textAreaDebug", scene, "TextArea");
				
				_textAreaDebug->setMetricsMode("pixel");
				_textAreaDebug->setPosition(0, 0);
				_textAreaDebug->setDimensions( entityInfo->getFloatAttribute("hudPanelDebugWidth")*relativeWidth,
			entityInfo->getFloatAttribute("hudPanelDebugHeight")*relativeHeight );

				_textAreaDebug->setText("Debug");
				_textAreaDebug->setTextSize(entityInfo->getIntAttribute("hudPanelDebugFontSize"));
				_textAreaDebug->setFont("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				_panelDebug->addChild(_textAreaDebug);

		_overlayDebug->add2D( _panelDebug );

		_overlayDebug->setVisible(false);


		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(CMessage *message)
	{
		return message->getMessageType() == Message::HUD_LIFE
			|| message->getMessageType() == Message::HUD_SHIELD
			|| message->getMessageType() == Message::HUD_AMMO
			|| message->getMessageType() == Message::HUD_WEAPON
			|| message->getMessageType() == Message::HUD_SPAWN
			|| message->getMessageType() == Message::HUD_DEBUG
			|| message->getMessageType() == Message::HUD_DEBUG_DATA;

	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			case Message::HUD_LIFE: {
				hudLife(((CMessageHudLife*)message)->getLife());
				break;
			}
			case Message::HUD_SHIELD: {
				hudShield(((CMessageHudShield*)message)->getShield());
				break;
			}
			case Message::HUD_AMMO: {
				CMessageHudAmmo *aux = static_cast<CMessageHudAmmo *>(message);
				hudAmmo( aux->getAmmo(), aux->getWeapon());
				break;
			}
			case Message::HUD_WEAPON: {
				CMessageHudWeapon *aux = static_cast<CMessageHudWeapon *>(message);
				hudWeapon(aux->getAmmo(), aux->getWeapon());
				break;
			}
			case Message::HUD_SPAWN: {
				CMessageHudSpawn *aux = static_cast<CMessageHudSpawn *>(message);
				_spawnTime = aux->getTime();
				hudSpawn(aux->getTime());
				break;
			}
			case Message::HUD_DEBUG: {
				hudDebug();
				break;
			}
			case Message::HUD_DEBUG_DATA: {
				CMessageHudDebugData *aux = static_cast<CMessageHudDebugData *>(message); 
				hudDebugData(aux->getKey(),aux->getValue() );
				break;
			}
		}

	} // process

	void CHudOverlay::hudLife(int health){
		_panelElementsText[HEALTH]= health;
		std::stringstream sHealth;
		sHealth << health;
		_panelElementsTextArea[HEALTH]->setText(sHealth.str());
	}

	void CHudOverlay::hudShield(int shield){
		_panelElementsText[SHIELD] = shield;
		std::stringstream sShield;
		sShield << shield;
		_panelElementsTextArea[SHIELD]->setText(sShield.str());
		std::cout << "mensaje de escudo recibido y procesado" << std::endl;
	}

	void CHudOverlay::hudAmmo(int ammo, int weapon){

		if(weapon == _actualWeapon)
		{
			_panelElementsText[AMMO] = ammo;
			if(ammo != 0){
				if(!_weaponsBox[_actualWeapon][ACTIVE]->isVisible())
				{
					_weaponsBox[_actualWeapon][NO_AMMO]->setVisible(false);
					_weaponsBox[_actualWeapon][ACTIVE]->setVisible(true);
				}
			}else{
				if(_weaponsBox[_actualWeapon][ACTIVE]->isVisible())
				{
					_weaponsBox[_actualWeapon][ACTIVE]->setVisible(false);
					_weaponsBox[_actualWeapon][NO_AMMO]->setVisible(true);
				}
			}//fin else	ammo!=0
			std::stringstream sAmmo;
			sAmmo << ammo;
			_panelElementsTextArea[AMMO]->setText(sAmmo.str());
		}//fin weapon == _actualweapon
		else{
			if(!_weaponsBox[weapon][ACTIVE]->isVisible()){
				_weaponsBox[weapon][ACTIVE]->setVisible(true);
				_weaponsBox[weapon][NO_WEAPON]->setVisible(false);
				_weaponsBox[weapon][NO_AMMO]->setVisible(false);
			}
		}
	}

	void CHudOverlay::hudWeapon(int ammo, int weapon){

		hudParticle("partilcle");
		//if(weapon != _actualWeapon && _actualWeapon != 0)
		if(weapon != _actualWeapon)
		{
			_overlayWeapon3D[_actualWeapon]->setVisible(false);
			_overlayWeapon3D[weapon]->setVisible(true);

			_actualWeapon = weapon;
			_weaponsBox[_actualWeapon][ACTIVE]->setVisible(true);
			_weaponsBox[_actualWeapon][NO_AMMO]->setVisible(false);
			_weaponsBox[_actualWeapon][NO_WEAPON]->setVisible(false);

			
		}
		hudAmmo(ammo, weapon);
	}

	void CHudOverlay::hudSpawn(int spawmTime){
		
		if(_overlayPlay->isVisible()){
			_overlayPlay->setVisible(false);
			_overlayWeapon3D[_actualWeapon]->setVisible(false);
		}

		std::stringstream sSpawn;
		sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;
		_textAreaDie->setText(sSpawn.str());
		
		if(!_overlayDie->isVisible())
			_overlayDie->setVisible(true);
		if(spawmTime <= 0.1){

			_overlayDie->setVisible(false);
			//reset para volver a mostrar solo el arma inicial al hacer show
			for(int i=1; i<_numWeapons;++i){
				_weaponsBox[i][NO_WEAPON]->setVisible(true);
				_weaponsBox[i][ACTIVE]->setVisible(false);
				_weaponsBox[i][NO_AMMO]->setVisible(false);
			}
			_overlayPlay->setVisible(true);
			_overlayWeapon3D[HAMMER]->setVisible(true);
			_actualWeapon = HAMMER;
		}

	}

	void CHudOverlay::hudParticle(const std::string &nameParticle){
		
		/*
		Graphics::CParticle *particle = new Graphics::CParticle("particle", "SmokeParticles", true);
		
		Graphics::COverlay *overlayParticle = _server->createOverlay( "_overlay3DParticle");
		overlayParticle->add3D(particle, new Vector3(0,0,-20));
		*/
	}
	
	std::string CHudOverlay::toText(eWeaponIndex weapon){
		switch(weapon){
			case HAMMER: return "hammer";
				break;
			case SNIPER: return "sniper";
				break;
			case SHOTGUN: return "shotGun";
				break;
			case MINIGUN: return "miniGun";
				break;
			case GRENADELAUNCHER: return "grenadeLauncher";
				break;
			case ROCKETLAUNCHER: return "rocketLauncher";
				break;
			case PRIMARY_SKILL: return "primarySkill";
				break;
			case SECUNDARY_SKILL: return "secondarySkill";
				break;
			default: return "";
			}
	}

	std::string CHudOverlay::toText(eOverlayWeaponState state){
		switch(state){
			case ACTIVE: return "ACTIVE";
				break;
			case NO_AMMO: return "NO_AMMO";
				break;
			case NO_WEAPON: return "NO_WEAPON";
				break;
			default: return "";
			}
	}

	std::string CHudOverlay::toText(eOverlayElements element){
		switch(element){
			case HEALTH: return "HEALTH";
				break;
			case SHIELD: return "SHIELD";
				break;
			case AMMO: return "AMMO";
				break;
			default: return "";
			}
	}

	void CHudOverlay::hudDebug(){
		/*
		if(_overlayDebug->isVisible()){
			printf("\nBorrate! ");	
		}else{
			printf("\npintate! ");
		}
		*/

		_overlayDebug->setVisible(!_overlayDebug->isVisible());
	}

	void CHudOverlay::hudDebugData(const std::string &key, const std::string &value){
		
		

		if(_textDebug.find(key) == _textDebug.end()){
			std::pair<std::string, std::string> aux(key, value);

			_textDebug.insert(aux);
		}else{
			_textDebug.find(key)->second = value;
		}

		/*
		std::pair<std::string, std::string> res = _textDebug.insert(std::make_pair(key,value));
		if ( res.second.empty()) {
			res.second = value;	
		}
		*/
	}

	void CHudOverlay::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	
		if(_overlayDie->isVisible()){
			_acumSpawn += msecs;
			if(_acumSpawn>1000){
				hudSpawn((--_spawnTime));
				_acumSpawn = 0;
			}
		}

		if(_overlayDebug->isVisible())
		{
			
			_sDebug.str("");
			_sDebug.clear();

			_acumDebug+=msecs;
			// han pasado el tiempo para actualizar el fps
			if(_acumDebug>200)
			{		
				std::stringstream aux;
				aux << 1000.0f/(float)msecs;

				hudDebugData("FPS", aux.str());
				_acumDebug=0;
			}
			
			for (std::map<std::string,std::string>::iterator it=_textDebug.begin(); it!=_textDebug.end(); ++it){
				_sDebug << it->first << " => " << it->second << '\n';
			}

			Logic::CMessageHudDebugData *m=new Logic::CMessageHudDebugData();
			m->setKey("Posicion");
			m->setValue(_entity->getPosition());
			_entity->emitMessage(m);
		}

		_textAreaDebug->setText(_sDebug.str());
	}

	void CHudOverlay::deactivate(){
		_overlayPlay->setVisible(false);
	}

} // namespace Logic

