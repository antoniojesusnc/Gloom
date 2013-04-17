//---------------------------------------------------------------------------
// PlayerInfo.cpp
//---------------------------------------------------------------------------

/**
@file PlayerInfo.cpp

Contiene la implementaci�n de la clase PlayerInfo para el proyecto de logica.

@see Logic::CPlayerInfo

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#include "PlayerInfo.h"

namespace Logic {

	CPlayerInfo::CPlayerInfo(Net::NetID netId) : _netId(netId), 
											     _rank(0), 
												 _entityIdInitialized(false) {
		
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(Net::NetID netId, const std::string& nickname) : _netId(netId), 
																			  _rank(0),
																			  _name(nickname),
																			  _entityIdInitialized(false) {
		
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(const CPlayerInfo& rhs) : _name(rhs._name), 
													   _mesh(rhs._mesh), 
													   _clan(rhs._clan), 
													   _playerClass(rhs._playerClass),
													   _rank(rhs._rank),
													   _entityId(rhs._entityId),
													   _entityIdInitialized(rhs._entityIdInitialized),
													   _netId(rhs._netId),
													   _playersLoaded(rhs._playersLoaded) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo& CPlayerInfo::operator=(const CPlayerInfo& rhs) {
		if(this != &rhs) {
			_name = rhs._name;
			_mesh = rhs._mesh;
			_clan = rhs._clan;
			_playerClass = rhs._playerClass;
			_rank = rhs._rank;
			_entityId = rhs._entityId;
			_entityIdInitialized = rhs._entityIdInitialized;
			_netId = rhs._netId;
			_playersLoaded = rhs._playersLoaded;
		}

		return *this;
	}

	//______________________________________________________________________________

	std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo) {
	   out << "Nombre = " << playerInfo._name << std::endl;
	   out << "Clase = " << playerInfo._playerClass << std::endl;
	   out << "Mesh = " << playerInfo._mesh << std::endl;
	   out << "Clan = " << playerInfo._clan << std::endl;
	   out << "Rank = " << playerInfo._rank << std::endl;
	   out << "EntityID = " << playerInfo._entityId << std::endl;
	   out << "EntityIDInitialized = " << (playerInfo._entityIdInitialized ? "true" : "false") << std::endl;
	   out << "NetID = " << playerInfo._netId << std::endl;
	   out << "NetID's de jugadores cargados =";

	   std::set<Net::NetID>::iterator it = playerInfo._playersLoaded.begin();
	   for(; it != playerInfo._playersLoaded.end(); ++it) {
		   out << " " << *it;
	   }

	   return out;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getName() {
		return _name;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getPlayerClass() {
		return _playerClass;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getMesh() {
		return _mesh;
	}

	//______________________________________________________________________________

	std::pair<TEntityID, bool> CPlayerInfo::getEntityId() {
		return std::pair<TEntityID, bool>(_entityId, _entityIdInitialized);
	}

	//______________________________________________________________________________

	Net::NetID CPlayerInfo::getNetId() {
		return _netId;
	}

	//______________________________________________________________________________
	
	unsigned int CPlayerInfo::playersLoaded() {
		return _playersLoaded.size();
	}

	//______________________________________________________________________________
	
	void CPlayerInfo::setName(const std::string& name) {
		this->_name = name;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setPlayerClass(const std::string& playerClass) {
		this->_playerClass = playerClass;
	}

	//______________________________________________________________________________
		
	void CPlayerInfo::setMesh(const std::string& mesh) {
		this->_mesh = mesh;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setEntityId(TEntityID entityId) {
		this->_entityId = entityId;
		_entityIdInitialized = true;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setNetId(Net::NetID netId) {
		this->_netId = netId;
	}

	//______________________________________________________________________________

	bool CPlayerInfo::loadPlayer(Net::NetID playerNetId) {
		return _playersLoaded.insert(playerNetId).second;
	}

	//______________________________________________________________________________

	bool CPlayerInfo::unloadPlayer(Net::NetID playerNetId) {
		return _playersLoaded.erase(playerNetId) > 0;
	}

	//______________________________________________________________________________

	void CPlayerInfo::clearLoadedPlayers() {
		_playersLoaded.clear();
	}

};