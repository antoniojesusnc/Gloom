//---------------------------------------------------------------------------
// GameNetPlayersManager.h
//---------------------------------------------------------------------------

/**
@file GameNetPlayersManager.h

Contiene la declaracion de la clase GameNetPlayersManager para el proyecto de logica.

@see Logic::CGameNetPlayersManager

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_GameNetPlayersManager_H
#define __Logic_GameNetPlayersManager_H

#include <map>
#include <string>
#include "PlayerInfo.h"

namespace Net {
	/** ID de identificaci�n en la red. */
	typedef unsigned int NetID;
};

namespace Logic {

	/**
	Esta clase es la encargada de controlar toda la informaci�n l�gica 
	asociada a los jugadores conectados a una partida (nombre, mesh, 
	estadisticas etc).

	@ingroup LogicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/
	class CGameNetPlayersManager {
	public:
		
		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CGameNetPlayersManager* getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CGameNetPlayersManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada para activar la escucha.
		*/
		void activate();

		/**
		Funci�n llamada desactivar la escucha.
		*/
		void deactivate();

		/** Inserta un nuevo jugador */
		bool addPlayer(Net::NetID idPlayer, const std::string& name, const std::string& mesh);

		/** Devuelve un jugador si es que ha sido registrado */
		CPlayerInfo getPlayer(Net::NetID idPlayer);

	protected:
		/** Constructor de la clase */
		CGameNetPlayersManager();

		/** Destructor */
		~CGameNetPlayersManager();

	private:
		typedef std::pair<Net::NetID, CPlayerInfo> TConnectedPlayersPair;
		typedef std::map<Net::NetID, CPlayerInfo> TConnectedPlayersTable;

		/** Map que contiene pares Net::ID e info de los players */
		TConnectedPlayersTable _connectedPlayers;

		/** �nica instancia de la clase. */
		static CGameNetPlayersManager* _instance;
	};

};

#endif