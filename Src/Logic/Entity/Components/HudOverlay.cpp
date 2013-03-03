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

#include "Graphics/Server.h"
#include "Graphics/Camera.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
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

namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	CHudOverlay::~CHudOverlay(){
		delete _overlayPlay;
		delete _overlayDie;

		delete _textAreaDie;

		//delete _textBoxArea[3];

		// En vez de 4 deberia de ir el numero de armas pero no tengo cojones U.U
		//delete _weaponsBox[6][3];
	}

	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		
		_numWeapons = entityInfo->getIntAttribute("numWeapons");
	
		_server = Graphics::CServer::getSingletonPtr();


		int hudScreenWidth = entityInfo->getIntAttribute("hudScreenWidth");
		int hudScreenHeight = entityInfo->getIntAttribute("hudScreenHeight");

		int hudPanelInitialPositionX = entityInfo->getIntAttribute("hudPanelInitialPositionX");
		int hudPanelInitialPositionY = entityInfo->getIntAttribute("hudPanelInitialPositionY");
		int hudPanelSizeX = entityInfo->getIntAttribute("hudPanelSizeX");
		int hudPanelSizeY = entityInfo->getIntAttribute("hudPanelSizeY");

		float height = _server->getHeight();
		float width = _server->getWidth();

		float relativeWidth = width/hudScreenWidth;
		float relativeHeight = height/hudScreenHeight;

		////////////////Todo esto para la mira
         // Create an overlay
		_overlayPlay = _server->createOverlay( "_overlayPlay" );		
         
		// Create a panel de Mira
		_panelMira = _server->createOverlay("Mira",  "Panel" );
		float sizeCrossFire = entityInfo->getFloatAttribute("hudCross");
		float positionCrossFire = 0.5f-((sizeCrossFire/2)/100) ;
        _panelMira->setPosition( positionCrossFire,positionCrossFire);
		_panelMira->setDimensions( sizeCrossFire/100, sizeCrossFire/100 );
        _panelMira->setMaterial("hudMira");

		_overlayPlay->add2D( _panelMira );
         // Add the panel to the overlay

		int x = hudPanelInitialPositionX;
		int y = hudPanelInitialPositionY;

/*		
		for(int i=0; i< NONE; ++i)
		{
			eWeaponIndex current = (eWeaponIndex)i;
			std::string currentOnText = toText(current);
		
			_panelWeapon[current] = _server->createOverlay( "PanelWeapon" + currentOnText, "Panel");
			_panelWeapon[current]->setMetricsMode("pixel");
			_panelWeapon[current]->setPosition( x*relativeWidth, y*relativeHeight);
			_panelWeapon[current]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeX );
			_panelWeapon[current]->setMaterial("cuadroArmas");

			_overlayPlay->add2D( _panelWeapon[current]);

			for(int j=0; j<=NO_WEAPON; ++j){
				eOverlayState currentState = (eOverlayState)j;

				std::string currentStateOnText = toText(currentState);

					//Aqui inicio el cuadro de ACTIVE, NO_AMMO, NO_WEAPON
				_weaponsBox[current][currentState] = _server->createOverlay( "_weaponsBox["+currentOnText+"]["+currentStateOnText+"]", "Panel");
				_weaponsBox[current][currentState]->setMetricsMode("pixel");
				_weaponsBox[current][currentState]->setPosition( x*relativeWidth, y*relativeHeight);
				_weaponsBox[current][currentState]->setDimensions( relativeWidth*hudPanelSizeX, relativeHeight*hudPanelSizeY );
				_weaponsBox[current][currentState]->setMaterial(currentOnText +"_"+ currentOnText);
				_weaponsBox[current][currentState]->setVisible(false);

				_overlayPlay->add2D( _weaponsBox[current][ACTIVE] );
			}
				_weaponsBox[current][NO_WEAPON]->setVisible(true);

			// Actualizo la posicion de los paneles
			x += hudPanelSizeX;
			// La posicion y no debe cambiar, ya que todo ira a la misma altura.
			//y += hudPanelSizeY;

		 }
		
	*/	
		////// panel PanelWeapon cuadro principal
		Graphics::COverlay *panelWeapon1 = _server->createOverlay( "PanelWeapon1", "Panel");
		panelWeapon1->setMetricsMode("pixel");
		panelWeapon1->setPosition( 9*relativeWidth, 29*relativeHeight);
		panelWeapon1->setDimensions( relativeWidth*2, relativeHeight*2 );
		
        panelWeapon1->setMaterial("cuadroArmas");

		_overlayPlay->add2D( panelWeapon1);
		
				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][ACTIVE] = _server->createOverlay( "_weaponsBox[HAMMER][ACTIVE]", "Panel");
			_weaponsBox[HAMMER][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[HAMMER][ACTIVE]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][ACTIVE]->setMaterial("KATANA_ACTIVE");
			_weaponsBox[HAMMER][ACTIVE]->setVisible(true);

			_overlayPlay->add2D( _weaponsBox[HAMMER][ACTIVE] );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][NO_AMMO] = _server->createOverlay("_weaponsBox[HAMMER][NO_AMMO]", "Panel"  );
			_weaponsBox[HAMMER][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[HAMMER][NO_AMMO]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][NO_AMMO]->setMaterial("KATANA_ACTIVE");
			_weaponsBox[HAMMER][NO_AMMO]->setVisible(true);

			_overlayPlay->add2D( _weaponsBox[HAMMER][NO_AMMO] );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][NO_WEAPON] = _server->createOverlay( "_weaponsBox[HAMMER][NO_WEAPON]", "Panel" );
			_weaponsBox[HAMMER][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[HAMMER][NO_WEAPON]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][NO_WEAPON]->setMaterial("KATANA_ACTIVE");
			_weaponsBox[HAMMER][NO_WEAPON]->setVisible(true);

			_overlayPlay->add2D( _weaponsBox[HAMMER][NO_WEAPON] );


		////// panel PanelWeapon2 cuadro principal
		Graphics::COverlay* panelWeapon2 = _server->createOverlay( "panelWeapon2", "Panel");
		panelWeapon2->setMetricsMode("pixel");
		panelWeapon2->setPosition( 11*relativeWidth, 29*relativeHeight);
		panelWeapon2->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon2->setMaterial("cuadroArmas");

		_overlayPlay->add2D( panelWeapon2 );

			 ////// panel PanelWeapon2 activa
			_weaponsBox[SNIPER][ACTIVE] = _server->createOverlay( "_weaponsBox[SNIPER][ACTIVE]", "Panel"  );
			_weaponsBox[SNIPER][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[SNIPER][ACTIVE]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][ACTIVE]->setMaterial("SNIPER_ACTIVE");
			_weaponsBox[SNIPER][ACTIVE]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[SNIPER][ACTIVE] );

			 ////// panel PanelWeapon2 BN
			_weaponsBox[SNIPER][NO_AMMO] = _server->createOverlay( "_weaponsBox[SNIPER][NO_AMMO]", "Panel" );
			_weaponsBox[SNIPER][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[SNIPER][NO_AMMO]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_AMMO]->setMaterial("SNIPER_NO_AMMO");
			_weaponsBox[SNIPER][NO_AMMO]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[SNIPER][NO_AMMO] );


			 ////// panel PanelWeapon2 SinBalas
			_weaponsBox[SNIPER][NO_WEAPON] = _server->createOverlay( "_weaponsBox[SNIPER][NO_WEAPON]", "Panel" );
			_weaponsBox[SNIPER][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[SNIPER][NO_WEAPON]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_WEAPON]->setMaterial("SNIPER_NO_WEAPON");
			_weaponsBox[SNIPER][NO_WEAPON]->setVisible(true);

			_overlayPlay->add2D( _weaponsBox[SNIPER][NO_WEAPON] );



		///// panel PanelWeapon3
		Graphics::COverlay* panelWeapon3 = _server->createOverlay("panelWeapon3","Panel");
		panelWeapon3->setMetricsMode("pixel");
		panelWeapon3->setPosition( 13*relativeWidth, 29*relativeHeight);
		panelWeapon3->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon3->setMaterial("cuadroArmas");
		_overlayPlay->add2D( panelWeapon3 );

				///// panel PanelWeapon3 Active
			_weaponsBox[SHOTGUN][ACTIVE] = _server->createOverlay( "_weaponsBox[SHOTGUN][ACTIVE]", "Panel"  );
			_weaponsBox[SHOTGUN][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[SHOTGUN][ACTIVE]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][ACTIVE]->setMaterial("SHOTGUN_ACTIVE");
			_weaponsBox[SHOTGUN][ACTIVE]->setVisible(false);
			
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][ACTIVE] );

			
				///// panel PanelWeapon3 no ammo
			_weaponsBox[SHOTGUN][NO_AMMO] = _server->createOverlay( "_weaponsBox[SHOTGUN][NO_AMMO]", "Panel" );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[SHOTGUN][NO_AMMO]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMaterial("SHOTGUN_NO_AMMO");
			_weaponsBox[SHOTGUN][NO_AMMO]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[SHOTGUN][NO_AMMO] );
			
				///// panel PanelWeapon3 no weapon
			_weaponsBox[SHOTGUN][NO_WEAPON] = _server->createOverlay( "_weaponsBox[SHOTGUN][NO_WEAPON]", "Panel" );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[SHOTGUN][NO_WEAPON]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMaterial("SHOTGUN_NO_WEAPON");
			_weaponsBox[SHOTGUN][NO_WEAPON]->setVisible(true);
			
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][NO_WEAPON] );
			
			

			

		///// panel PanelWeapon4
		Graphics::COverlay* panelWeapon4 = _server->createOverlay( "panelWeapon4","Panel");
		panelWeapon4->setMetricsMode("pixel");
		panelWeapon4->setPosition( 15*relativeWidth, 29*relativeHeight);
		panelWeapon4->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon4->setMaterial("cuadroArmas");

		Graphics::COverlay* textAreaDummy = _server->createOverlay("textAreaDummy", "TextArea");
				
				
				
				textAreaDummy->setMetricsMode("pixel");
				textAreaDummy->setPosition(0.5, 0.5);
				textAreaDummy->setDimensions(10, 10);
				textAreaDummy->setText("pepe");
				textAreaDummy->setTextSize(16);
				textAreaDummy->setFont("fuenteSimple");

				
				panelWeapon4->addChild(textAreaDummy);

				///// panel PanelWeapon4 Active
			_weaponsBox[MINIGUN][ACTIVE] = _server->createOverlay( "_weaponsBox[MINIGUN][ACTIVE]", "Panel"  );
			_weaponsBox[MINIGUN][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[MINIGUN][ACTIVE]->setPosition( 15*relativeWidth, 29*relativeHeight);
			_weaponsBox[MINIGUN][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[MINIGUN][ACTIVE]->setMaterial("MINIGUN_ACTIVE");
			_weaponsBox[MINIGUN][ACTIVE]->setVisible(false);
			
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][ACTIVE] );

			
				///// panel PanelWeapon4 no ammo
			_weaponsBox[MINIGUN][NO_AMMO] = _server->createOverlay( "_weaponsBox[MINIGUN][NO_AMMO]", "Panel" );
			_weaponsBox[MINIGUN][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[MINIGUN][NO_AMMO]->setPosition( 15*relativeWidth, 29*relativeHeight);
			_weaponsBox[MINIGUN][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[MINIGUN][NO_AMMO]->setMaterial("MINIGUN_NO_AMMO");
			_weaponsBox[MINIGUN][NO_AMMO]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[MINIGUN][NO_AMMO] );
			
				///// panel PanelWeapon4 no weapon
			_weaponsBox[MINIGUN][NO_WEAPON] = _server->createOverlay( "_weaponsBox[MINIGUN][NO_WEAPON]", "Panel" );
			_weaponsBox[MINIGUN][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[MINIGUN][NO_WEAPON]->setPosition( 15*relativeWidth, 29*relativeHeight);
			_weaponsBox[MINIGUN][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[MINIGUN][NO_WEAPON]->setMaterial("MINIGUN_NO_WEAPON");
			_weaponsBox[MINIGUN][NO_WEAPON]->setVisible(true);
			
			_overlayPlay->add2D( _weaponsBox[MINIGUN][NO_WEAPON] );
			
		_overlayPlay->add2D( panelWeapon4 );


		///// panel PanelWeapon5
		Graphics::COverlay* panelWeapon5 = _server->createOverlay( "panelWeapon5","Panel");
		panelWeapon5->setMetricsMode("pixel");
		panelWeapon5->setPosition( 17*relativeWidth, 29*relativeHeight);
		panelWeapon5->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon5->setMaterial("cuadroArmas");

				///// panel PanelWeapon3 Active
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE] = _server->createOverlay( "_weaponsBox[ROCKET_LAUNCHER][ACTIVE]", "Panel"  );
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE]->setPosition( 17*relativeWidth, 29*relativeHeight);
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE]->setMaterial("weapon5");
			_weaponsBox[ROCKET_LAUNCHER][ACTIVE]->setVisible(false);
			
			_overlayPlay->add2D( _weaponsBox[ROCKET_LAUNCHER][ACTIVE] );

			
				///// panel PanelWeapon3 no ammo
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO] = _server->createOverlay( "_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]", "Panel" );
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]->setPosition( 17*relativeWidth, 29*relativeHeight);
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]->setMaterial("weapon5BN");
			_weaponsBox[ROCKET_LAUNCHER][NO_AMMO]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[ROCKET_LAUNCHER][NO_AMMO] );
			
				///// panel PanelWeapon3 no weapon
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON] = _server->createOverlay( "_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]", "Panel" );
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]->setPosition( 17*relativeWidth, 29*relativeHeight);
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]->setMaterial("weapon5SinBalas");
			_weaponsBox[ROCKET_LAUNCHER][NO_WEAPON]->setVisible(true);
			
			_overlayPlay->add2D( _weaponsBox[ROCKET_LAUNCHER][NO_WEAPON] );
			
		_overlayPlay->add2D( panelWeapon5 );


		///// panel PanelWeapon6
		Graphics::COverlay* panelWeapon6 = _server->createOverlay( "panelWeapon6","Panel");
		panelWeapon6->setMetricsMode("pixel");
		panelWeapon6->setPosition( 19*relativeWidth, 29*relativeHeight);
		panelWeapon6->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon6->setMaterial("cuadroArmas");

				///// panel PanelWeapon3 Active
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE] = _server->createOverlay( "_weaponsBox[GRENADE_LAUNCHER][ACTIVE]", "Panel"  );
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE]->setMetricsMode("pixel");
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE]->setPosition( 19*relativeWidth, 29*relativeHeight);
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE]->setMaterial("weapon6");
			_weaponsBox[GRENADE_LAUNCHER][ACTIVE]->setVisible(false);
			
			_overlayPlay->add2D( _weaponsBox[GRENADE_LAUNCHER][ACTIVE] );

			
				///// panel PanelWeapon3 no ammo
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO] = _server->createOverlay( "_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]", "Panel" );
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]->setMetricsMode("pixel");
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]->setPosition( 19*relativeWidth, 29*relativeHeight);
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]->setMaterial("weapon6BN");
			_weaponsBox[GRENADE_LAUNCHER][NO_AMMO]->setVisible(false);

			_overlayPlay->add2D( _weaponsBox[GRENADE_LAUNCHER][NO_AMMO] );
			
				///// panel PanelWeapon3 no weapon
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON] = _server->createOverlay( "_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]", "Panel" );
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]->setMetricsMode("pixel");
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]->setPosition( 19*relativeWidth, 29*relativeHeight);
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]->setMaterial("weapon6SinBalas");
			_weaponsBox[GRENADE_LAUNCHER][NO_WEAPON]->setVisible(true);
			
			_overlayPlay->add2D( _weaponsBox[GRENADE_LAUNCHER][NO_WEAPON] );
			
		_overlayPlay->add2D( panelWeapon6 );

		

		///// panel health
		Graphics::COverlay *panelHealth = _server->createOverlay("panelHealth",  "Panel" );
		panelHealth->setMetricsMode("pixel");
		panelHealth->setPosition( 3*relativeWidth, 29*relativeHeight);
		panelHealth->setDimensions( relativeWidth*1.5f, relativeHeight*1.5f );
        panelHealth->setMaterial("hudHealth");

		_textBoxArea[HEALTH] = _server->createOverlay("textAreaPanelHealth", "TextArea");
				
		
				_textBoxArea[HEALTH]->setMetricsMode("pixel");
				_textBoxArea[HEALTH]->setPosition(-relativeWidth*1.5, 0);
				_textBoxArea[HEALTH]->setDimensions(relativeWidth*2, relativeHeight*2);

					std::stringstream sHealth;//create a stringstream
					sHealth << _health;//add number to the stream
				_textBoxArea[HEALTH]->setText(sHealth.str());
				_textBoxArea[HEALTH]->setTextSize(46);
				_textBoxArea[HEALTH]->setFont("fuenteSimple");
				//el metodo set colour no me lo he definido en COverlay
				//_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);

				//panelShield->addChild(textAreaHealth);
				
				panelHealth->addChild(_textBoxArea[HEALTH]);

		_overlayPlay->add2D( panelHealth );
         // Add the panel to the overlay
        
		
		///// panel shield
		Graphics::COverlay* panelShield = _server->createOverlay("panelShield", "Panel");
		panelShield->setMetricsMode("pixel");
		panelShield->setPosition( 7*relativeWidth, 29*relativeHeight);
		panelShield->setDimensions( relativeWidth*1.5f, relativeHeight*1.5f );
        panelShield->setMaterial("hudShield");


				_textBoxArea[SHIELD] = _server->createOverlay("textAreaPanelShield", "TextArea");
				
				_textBoxArea[SHIELD]->setMetricsMode("pixel");
				_textBoxArea[SHIELD]->setPosition(-relativeWidth*1.5, 0);
				_textBoxArea[SHIELD]->setDimensions(relativeWidth*2, relativeHeight*2);

					std::stringstream sShield;//create a stringstream
					sShield << _shield;//add number to the stream
				_textBoxArea[SHIELD]->setText(sShield.str());
				_textBoxArea[SHIELD]->setTextSize(46);
				_textBoxArea[SHIELD]->setFont("fuenteSimple");
				//_textBoxArea[SHIELD]->setColour(Ogre::ColourValue::White);

				panelShield->addChild(_textBoxArea[SHIELD]);
				_overlayPlay->add2D( panelShield );

		

		///// panel AMMO
		Graphics::COverlay* panelAmmo = _server->createOverlay("panelAmmo", "Panel");
		panelAmmo->setMetricsMode("pixel");
		panelAmmo->setPosition( 24*relativeWidth, 29*relativeHeight);
		panelAmmo->setDimensions( relativeWidth*0.5, relativeHeight*1.5 );
        panelAmmo->setMaterial("hudAmmo");

		
				_textBoxArea[AMMO] = _server->createOverlay("textAreaPanelAmmo", "TextArea");
				
				_textBoxArea[AMMO]->setMetricsMode("pixel");
				_textBoxArea[AMMO]->setPosition(-relativeWidth*2, 0);
				_textBoxArea[AMMO]->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream sAmmo;
					sAmmo << _ammo;
				_textBoxArea[AMMO]->setText(sAmmo.str());
				_textBoxArea[AMMO]->setTextSize(46);
				_textBoxArea[AMMO]->setFont("fuenteSimple");
				//_textBoxArea[AMMO]->setColour(Ogre::ColourValue::White);

				panelAmmo->addChild(_textBoxArea[AMMO]);
				_overlayPlay->add2D( panelAmmo );

		for(int i = 1; i < _numWeapons; ++i){
			_weaponsBox[i][NO_AMMO]->setVisible(false);
			_weaponsBox[i][ACTIVE]->setVisible(false);
		}
		
		//_overlayPlay->add3D("AK47", "AK47.mesh", new Vector3(0,0,-5));
		
		_overlayPlay->setVisible(true);



		 //////////////////////////////////////AQUI ME CREO EL OVERLAY PARA CUANDO SE MUERA

		_overlayDie = _server->createOverlay( "_overlayDie" );

		Graphics::COverlay* panelDie = _server->createOverlay("panelDie",  "Panel");
		panelDie->setMetricsMode("pixel");
		panelDie->setPosition( 5*relativeWidth, 12*relativeHeight);
		panelDie->setDimensions( relativeWidth*10, relativeHeight*10 );
        //panelDie->setMaterialName("cuadroArmas");

		_textAreaDie = _server->createOverlay("_textAreaDie", "TextArea");
				
				_textAreaDie->setMetricsMode("pixel");
				_textAreaDie->setPosition(0.5, 0.5);
				_textAreaDie->setDimensions(10, 10);

				_textAreaDie->setText("MUERE");
				_textAreaDie->setTextSize(46);
				_textAreaDie->setFont("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				panelDie->addChild(_textAreaDie);

		_overlayDie->add2D( panelDie );

		//_overlayDie->show();
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(CMessage *message)
	{
		return message->getMessageType() == Message::HUD_LIFE
			|| message->getMessageType() == Message::HUD_SHIELD
			|| message->getMessageType() == Message::HUD_AMMO
			|| message->getMessageType() == Message::HUD_WEAPON
			|| message->getMessageType() == Message::HUD_SPAWN;

	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::HUD_LIFE: hudLife(((CMessageHudLife*)message)->getLife());
			break;
		case Message::HUD_SHIELD: hudShield(((CMessageHudShield*)message)->getShield());
			break;
		case Message::HUD_AMMO: hudAmmo( ((CMessageHudAmmo*)message)->getAmmo(), ((CMessageHudAmmo*)message)->getWeapon());
			break;
		case Message::HUD_WEAPON: hudWeapon(((CMessageHudWeapon*)message)->getAmmo(), ((CMessageHudWeapon*)message)->getWeapon());
			break;
		case Message::HUD_SPAWN: hudSpawn(((CMessageHudSpawn*)message)->getTime());
			break;
		}

	} // process

	void CHudOverlay::hudLife(int health){
		_health = health;
		std::stringstream sHealth;
		sHealth << _health;
		_textBoxArea[HEALTH]->setText(sHealth.str());
	}

	void CHudOverlay::hudShield(int shield){
		_shield = shield;

		std::stringstream sShield;
		sShield << _shield;
		_textBoxArea[SHIELD]->setText(sShield.str());
	}

	void CHudOverlay::hudAmmo(int ammo, int weapon){

		if(weapon == _actualWeapon)
		{
			_ammo = ammo;
			if(_ammo != 0){
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
			sAmmo << _ammo;
			_textBoxArea[AMMO]->setText(sAmmo.str());
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

		
		//if(weapon != _actualWeapon && _actualWeapon != 0)
		if(weapon != _actualWeapon)
		{
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
		}

	}
	
	std::string CHudOverlay::toText(eWeaponIndex weapon){
		switch(weapon){
			case HAMMER: return "HAMMER";
				break;
			case SNIPER: return "SNIPER";
				break;
			case SHOTGUN: return "SHOTGUN";
				break;
			case MINIGUN: return "MINIGUN";
				break;
			case GRENADE_LAUNCHER: return "GRENADE_LAUNCHER";
				break;
			case ROCKET_LAUNCHER: return "ROCKET_LAUNCHER";
				break;
			}
	}

	std::string CHudOverlay::toText(eOverlayState state){
		switch(state){
			case ACTIVE: return "ACTIVE";
				break;
			case NO_AMMO: return "NO_AMMO";
				break;
			case NO_WEAPON: return "NO_WEAPON";
				break;
			}
	}

} // namespace Logic

