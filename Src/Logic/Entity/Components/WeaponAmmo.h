//---------------------------------------------------------------------------
// Ammo.h
//---------------------------------------------------------------------------

/**
@file Ammo.h

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_WeaponAmmo_H
#define __Logic_WeaponAmmo_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup

	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/

	class IWeaponAmmo : public IComponent {
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deber�n llamar a este constructor
		con el nombre de su arma.

		@param weaponName Nombre del arma implementada por la clase derivada.
		*/
		IWeaponAmmo(const std::string& weaponName);

		//__________________________________________________________________

		/** Destructor. */
		virtual ~IWeaponAmmo();


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
		virtual void primaryFire();

		//__________________________________________________________________
		
		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo secundario.
		*/
		virtual void secondaryFire();

		//__________________________________________________________________

		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopPrimaryFire();

		//__________________________________________________________________

		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		secundario.
		*/
		virtual void stopSecondaryFire();

		//__________________________________________________________________

		/**
		Incrementar la munici�n de un arma.

		@param weapon Identificador del arma.
		@param ammo Munici�n a incrementar.
		@param iAmCath Para el HUD.
		*/
		virtual void addAmmo(int weapon, int ammo, bool iAmCatch);

		//__________________________________________________________________

		/**
		Resetea la munici�n.
		*/
		virtual void resetAmmo();

		//__________________________________________________________________

		/**
		M�todo virtual puro invocado cuando se decrementa el tiempo de cooldown del arma.
		El cliente es responsable de decrementar los cooldowns de su arma en 
		el porcentaje dado por par�metro.

		@param percentage Tanto por ciento del 1 al 100 en el que se decrementa
		el cooldown del arma. Si su valor es 0, significa que debemos resetear
		los cooldowns del arma a su valor por defecto.
		*/
		virtual void reduceCooldown(unsigned int percentage) = 0;


		virtual void amplifyDamage(unsigned int percentage);

	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Llamado cuando el arma pasa a ser activa.
		*/
		virtual void onAvailable();


		/**
		Llamado cuando el arma pasa a ser inactiva.
		*/
		virtual void onBusy();

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
		Decrementa la munici�n en base al par�metro dado y actualiza la munici�n
		que aparece en el HUD.

		@param Cantidad de munici�n a reducir. Por defecto es 1.
		*/
		void decrementAmmo(unsigned int ammoSpent = 1);

		//__________________________________________________________________

		/**
		Pinta un decal dada una entidad y una posicion

		@deprecated Temporalmente est� implementada como un helper, pero
		en el futuro est� funci�n deber�a formar parte del componente gr�fico.

		@param pEntity Entidad sobre la que queremos pintar el decal.
		@param vPos Posici�n en la que queremos pintar el decal.
		*/
		void drawDecal(Logic::CEntity* pEntity, Vector3 vPos);

		//__________________________________________________________________

		/**
		Establece la guarda que indica cuando se puede hacer
		uso del disparo primario.
		*/
		virtual bool canUsePrimaryFire() = 0;

		//__________________________________________________________________
		
		/**
		Establece la guarda que indica cuando se puede hacer
		uso del disparo secundario.
		*/
		virtual bool canUseSecondaryFire() = 0;


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		bool _primaryFireIsActive;

		bool _secondaryFireIsActive;

		/** Cuanta munici�n puede llevar este arma como m�ximo. */
		unsigned int _maxAmmo;
		
		/** Cuanta munici�n tenemos actualmente en este arma. */
		unsigned int _currentAmmo;
		
		// creo q las dos siguientes variables no son necesarias
		/** Radio de la c�psula del personaje */
		float _capsuleRadius;
		
		/** Altura desde la que sale el disparo. */
		float _heightShoot;
		
		/** Enumerado que indica cual es el identificador de arma. */
		WeaponType::Enum _weaponID;

		/** Nombre del arma con el formato: weapon + <nombre arma>.*/
		std::string _weaponName;

		IComponent *_friend;
	}; // class IWeapon

} // namespace Logic

#endif // __Logic_Weapon_H
