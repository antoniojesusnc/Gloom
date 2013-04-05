/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Antonio Jesus
@date Febrero, 2013
*/
#ifndef __Logic_HudOverlay_H
#define __Logic_HudOverlay_H

#include "Graphics/Server.h"
#include "Graphics/Entity.h"
#include "Logic/Entity/Component.h"

/*
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
*/

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla el hud, mediante overlays
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Febrero, 2013
*/
	class CHudOverlay : public IComponent
	{
		DEC_FACTORY(CHudOverlay);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHudOverlay() : IComponent(), _health(0), _shield(0), _ammo(1), _actualWeapon(0), _numWeapons(0), _panelMira(0), _temporal(0){
			for(int i=0;i<3;++i){
				_panelElementsText[i]=1;
			}
		}
		

		virtual ~CHudOverlay();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);
		
		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Lo uso para cambiar los valores del debug

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);


		virtual void deactivate();
	protected:

		int _numWeapons;

		enum eWeaponIndex { HAMMER, SNIPER, SHOTGUN, MINIGUN, GRENADELAUNCHER, ROCKETLAUNCHER, NONE };
		enum eOverlayWeaponState { ACTIVE, NO_AMMO, NO_WEAPON };
		enum eOverlayElements {HEALTH, SHIELD, AMMO };

		void hudLife(int health);
		void hudShield(int shield);
		void hudWeapon(int ammo, int weapon);
		void hudAmmo(int ammo, int weapon);
		void hudSpawn(int spawn);

		void hudParticle(const std::string &nameParticle);

		void hudDebug();

		std::string toText(eWeaponIndex weapon);
		std::string toText(eOverlayWeaponState state);
		std::string toText(eOverlayElements element);


		int _health;
		int _shield;
		int _ammo;
		int _actualWeapon;

		Graphics::CServer* _server;
		
		/** 
		overlay principal, albergar� los paneles de armas, y el inidicador de vida, municion y armadura
		*/
		Graphics::COverlay *_overlayPlay;

		/** 
		Overlay que se mostrara cuando se muere el usuario
		*/
		Graphics::COverlay *_overlayDie;
		
		/**
		Texto que se muestra cuando el jugador se muere
		*/
		Graphics::COverlay *_textAreaDie;

		/**
		Overlays de los siguiente elementos, HEALTH, SHIELD, AMMO
		*/
		Graphics::COverlay *_panelElements[3];
		/** 
		OverlaysText  de los siguiente elementos, HEALTH, SHIELD, AMMO
		*/
		Graphics::COverlay *_panelElementsTextArea[3];
		/**
		Textos que se muestran por pantalla en los overlaysElements
		*/
		int _panelElementsText[3];

		/**
		Maneja el overlay de la mira.
		*/
		Graphics::COverlay *_panelMira;

		/** 
		Gestiona los paneles de armas basicas (el 6 esta a hierro, deberia se ser el num de armas, pero bueno)
		*/
		Graphics::COverlay *_panelWeapon[6];
		/**
		Gestion los paneles de las armas (6) en este caso, en cada uno de sus estados: ACTIVE, NO_AMMO, NO_WEAPON
		*/
		Graphics::COverlay *_weaponsBox[6][3];


		//////////////////////Gestion de armas
		Graphics::COverlay *_overlayWeapon3D[6];

		/**
		Aqui guardare un puntero a cada entidad, para poder moverla y rotarla
		*/
		Graphics::CEntity *_weaponsEntities[6];


		/**
		
		*/
		Graphics::COverlay *_overlayDebug;
		Graphics::COverlay *_panelDebug;
		Graphics::COverlay *_textAreaDebug;


		int _temporal;
	}; // class CHudOverlay

	REG_FACTORY(CHudOverlay);

} // namespace Logic

#endif // __Logic_Life_H
