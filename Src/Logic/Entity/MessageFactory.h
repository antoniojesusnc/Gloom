/**
@file MessageFactory.h

Contiene la declaraci�n de la clase CMessageFactory, Singleton que almacena 
los punteros a funci�n de funciones de creaci�n componentes (CMessage).

@see Logic::CMessageFactory
@see BaseSubsystems::CFactory
@see Logic::CComponent

@author Rub�n Mulero
@date Febrero, 2013
*/
#ifndef __Logic_MessageFactory_H
#define __Logic_MessageFactory_H

#include <string>
#include <map>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CMessage;
}

// Declaraci�n de la clase
namespace Logic
{
	/**
	Factor�a de componentes. Extiende la plantilla BaseSubsystems::CFactory
	para CMessage haciendo de ella un singleton por lo que solo habr� una 
	instancia de esta clase.
	<p>
	Es usada para almacenar componentes 
	en Logic::CEntityFactory donde todo el registro de componentes se realiza 
	utilizando macros del preprocesador declarados en Logic::CComponent.

	@ingroup logicGroup
	@ingroup mapGroup

	@author Rub�n Mulero
	@date Febrero, 2013
	*/
	
	class CMessageFactory
	{

	protected:
		
		/** 
		Puntero a funci�n.  
		*/
		typedef CMessage* (*FunctionPointer)();
	public:

		/**
		Devuelve la �nica instancia de la clase CMessageFactory.
		
		@return �nica instancia de la clase CMessageFactory.
		*/
		static CMessageFactory* getSingletonPtr();

		/**
		Destructor.
		*/
		~CMessageFactory();

		/**
		A�ade una nueva funci�n a la tabla de la factor�a.

		@param funcion Funci�n de creaci�n del objeto.
		@param name Nombre con el que nos referiremos a la funci�n para
		la futura creaci�n del objeto.
		*/
		void add(FunctionPointer function, const std::string& name);

		/**
		Si una funci�n ya est� contenida en la tabla.

		@param name Nombre de la clase del objeto que se quiere saber si 
		se puede crear.
		@return true si se encuentra en la tabla.
		*/
		bool has(const std::string& name) const;

		/**
		Crea una nueva instancia de la clase requerida.

		@param name Nombre de la clase del objeto que se quiere crear.
		@return Nueva instancia de la clase. 0 si no est� en la tabla.
		*/
		
		CMessage * create(const std::string& name) const;


		/**
		Crea una nueva instancia de la clase requerida.

		@param name Nombre de la clase del objeto que se quiere crear.
		@return Nueva instancia de la clase. 0 si no est� en la tabla.
		*/
		
		CMessage * create(int name) const;

	protected:
		/**
		Constructor.
		*/
		CMessageFactory ();

		typedef std::pair <int,FunctionPointer> TStringFunctionPointerPair;
		typedef std::map<int,FunctionPointer> TFunctionPointerMap;

		TFunctionPointerMap _table;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CMessageFactory* _instance;

		

	}; // class CMessageFactory

} // namespace Logic

#endif // __Logic_MessageFactory_H
