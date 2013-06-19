//---------------------------------------------------------------------------
// Ammo.h
//---------------------------------------------------------------------------

/**
@file Ammo.h

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_SpellAmmo_H
#define __Logic_SpellAmmo_H

#include "Logic/Entity/Component.h"
#include "SpellType.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup

	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class ISpellAmmo : public IComponent {
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deber�n llamar a este constructor
		con el nombre de su arma.

		@param spellName Nombre del arma implementada por la clase derivada.
		*/
		ISpellAmmo(const std::string& spellName);

		//__________________________________________________________________

		/** Destructor. */
		virtual ~ISpellAmmo();


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

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopSpell();

		//__________________________________________________________________

		/**
		M�todo virtual puro invocado cuando se decrementa el tiempo de cooldown del arma.
		El cliente es responsable de decrementar los cooldowns de su arma en 
		el porcentaje dado por par�metro.

		@param percentage Tanto por ciento del 1 al 100 en el que se decrementa
		el cooldown del arma. Si su valor es 0, significa que debemos resetear
		los cooldowns del arma a su valor por defecto.
		*/
		virtual void reduceCooldown(unsigned int percentage) { };

		virtual void addAmmo() {};
	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Llamado cuando el arma pasa a ser activa.
		Si es hechizo es pasivo hara aqui su accion.
		*/
		virtual void onWake();


		/**
		Llamado cuando el arma pasa a ser inactiva.
		*/
		virtual void onSleep();


		virtual void onActivate();


		virtual void onDeactivate();
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

		/**
		Establece si se puede usar el conjuro o no.
		Redefinir para los hechizos no pasivos
		*/
		virtual bool canUseSpell() { return true; };

		//__________________________________________________________________

		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		/** Enumerado que indica cual es el identificador de arma. */
		SpellType::Enum _spellID;

		/** Nombre del arma con el formato: spell + <nombre arma>.*/
		std::string _spellName;

		/** variable que controlara si es conjuro primario o secundario. */
		bool _isPrimarySpell;

		/** Variable para controlar si un hechizo es activo o pasivo */
		bool _isPassive;

		unsigned int _friends;
		IComponent *_friend[2];
	}; // class ISpell

} // namespace Logic

#endif // __Logic_Spell_H
