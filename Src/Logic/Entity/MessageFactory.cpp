/**
@file ComponentFactory.cpp

Contiene la implementaci�n de la clase CMessageFactory, Singleton que almacena 
los punteros a funci�n de funciones de creaci�n componentes (IComponent).

@see Logic::CMessageFactory
@see BaseSubsystems::CFactory
@see Logic::CComponent

@author David Llans�
@date Julio, 2010
*/
#include "MessageFactory.h"
#include "BaseSubsystems\Math.h"
#include "Logic/Messages/Message.h"

#include <cassert>

namespace Logic {

	CMessageFactory* CMessageFactory::_instance = 0;

	//--------------------------------------------------------

	CMessageFactory::CMessageFactory()
	{
		_instance = this;

	} // CMessageFactory

	//--------------------------------------------------------

	CMessageFactory::~CMessageFactory()
	{
		_instance = 0;
		std::map<int, FunctionPointer>::const_iterator it = _table.begin();
		std::map<int, FunctionPointer>::const_iterator it2;
		
		while( it != _table.end() )
		{
			it2 = it;
			it++;
			_table.erase(it2);
		}

	} // ~CMessageFactory
	
	//--------------------------------------------------------

	CMessageFactory* CMessageFactory::getSingletonPtr()
	{
		if(!_instance)
			new CMessageFactory();

		return _instance;

	} // Init

	
	void CMessageFactory::add(FunctionPointer function, const std::string& name)
	{
		TStringFunctionPointerPair element(Math::CRC(name),function);
		_table.insert(element);

	} // add

	//--------------------------------------------------------

	bool CMessageFactory::has(const std::string& name) const
	{
		return _table.count(Math::CRC(name)) > 0;

	} // has

	//--------------------------------------------------------

	CMessage * CMessageFactory::create(const std::string& name) const
	{
		int n = Math::CRC(name);
		if(has(name))
		{
			TFunctionPointerMap::const_iterator it;
			it = _table.find(n);
			if( it != _table.end() )
				return _table.find(n)->second();
		}
		throw new std::exception("No existe la funci�n de creaci�n que se solicit�.");

	} // create (string)

	CMessage * CMessageFactory::create(int name) const
	{
		
		if(_table.count(name) > 0)
		{
			TFunctionPointerMap::const_iterator it;
			it = _table.find(name);
			if( it != _table.end() )
				return _table.find(name)->second();
		}
		throw new std::exception("No existe la funci�n de creaci�n que se solicit�.");

	}//create (int)


} // namespace Logic
