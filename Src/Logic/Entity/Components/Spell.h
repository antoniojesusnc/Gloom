//---------------------------------------------------------------------------
// Spell.h
//---------------------------------------------------------------------------

/**
@file Spell.h

Contiene la declaraci�n de la interfaz com�n
a todas las armas.

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Mayo, 2013
*/
#ifndef __Logic_Spell_H
#define __Logic_Spell_H

#include "Logic/Entity/Component.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup

	Interfaz com�n a todas las armas del juego. El objetivo
	de esta clase es asegurarse de que todas siguen una interfaz
	com�n y sencilla que facilite la implementaci�n y la comprensi�n
	de las armas.

	Debido a que el funcionamiento de cada arma es realmente 
	�nico, hemos preferido evitar generalizar en esta clase.

	IMPORTANTE: Decrementar la munici�n y emitir los sonidos
	de disparo es responsabilidad del cliente. Las animaciones
	de las armas tambien son responsabilidad del cliente.

	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class ISpell : public IComponent {
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deber�n llamar a este constructor
		con el nombre de su arma.

		@param spellName Nombre del arma implementada por la clase derivada.
		*/
		ISpell(const std::string &spellName);

		//__________________________________________________________________

		/** Destructor. */
		virtual ~ISpell();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>MaxAmmo:</strong> Munici�n m�xima que puede llevar el arma. </li>
			<li><strong>ID:</strong> Identificador de arma. </li>
			<li><strong>physic_radius:</strong> Radio de la c�psula f�sica del personaje. </li>
			<li><strong>heightShoot:</strong> Altura desde donde sale el disparo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________


		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell() = 0;

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se muere.
		*/
		virtual void stopSpell() { /* Por defecto nada */ };

		//__________________________________________________________________

		void addDuration();
	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		virtual void onTick(unsigned int msecs);

		virtual void onWake();
		
		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================

		
		/**
		Helper para emitir un sonido dada una ruta.

		@param ruta Ruta del sonido que queremos reproducir.
		@param sound String para identificar el sonido??.
		@param notIfPlay ?�?
		*/
		void emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay = false);

		//__________________________________________________________________

	
		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================
		
		/** Nombre del arma con el formato: weapon + <nombre arma>.*/
		std::string _spellName;

		unsigned int _spellID;

		int _currentTime;
		unsigned int _duration;
	}; // class ISpell

} // namespace Logic

#endif // __Logic_Spell_H