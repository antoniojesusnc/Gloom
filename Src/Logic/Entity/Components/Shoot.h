//---------------------------------------------------------------------------
// Shoot.h
//---------------------------------------------------------------------------

/**
@file Shoot.h
 
 
@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_Shoot_H
#define __Logic_Shoot_H

#include "Logic/Entity/Component.h"

namespace Logic {

	/**
	@ingroup logicGroup

	Este componente define la interfaz que van a utilizar todas las armas
	del player. Cada arma es un nuevo componente que hereda de esta clase
	y que �nicamente debe preocuparse de redefinir el m�todo shoot, que 
	ser� el m�todo que se dispare cuando se detecte que el player ha 
	inicializado la acci�n de disparo.

	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CShoot : public IComponent {
		DEC_FACTORY(CShoot);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShoot() : IComponent(), 
				   _capsuleRadius(3.0f),
				   _canShoot(true), 
				   _coldDownTime(0), 
				   _temporal(0),
				   _nameWeapon(0), 
				   _currentAmmo(0)  {
		
			// No hay memoria dinamica que inicializar
		}

		//__________________________________________________________________

		/**
		Constructor parametrizado

		@param shoot Nombre del arma que vamos a inicializar.
		*/
		CShoot(const std::string &shoot) : IComponent(),  
										   _capsuleRadius(3.0f),
										   _canShoot(true),
										   _coldDownTime(0), 
										   _temporal(0), 
										   _nameWeapon(shoot), 
										   _currentAmmo(0) {
			
			// No hay memoria din�mica que inicializar
		}
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>physic_radius:</strong> Radio de la c�psula fisica del jugador. </li>
			<li><strong>heightShoot:</strong> Altura de la que sale el disparo. </li>
			<li><strong>(weaponName)+Name:</strong> Nombre del arma. </li>
			<li><strong>(weaponName)+Damage:</strong> Da�o que hace cada impacto del arma. </li>
			<li><strong>(weaponName)+Dispersion:</strong> Dispersion del arma. </li>
			<li><strong>(weaponName)+Distance:</strong> Distancia que alcanza el arma. </li>
			<li><strong>(weaponName)+NumberShoots:</strong> N�mero de balas por disparo. </li>
			<li><strong>(weaponName)+ColdDown:</strong> Cooldown del arma (tiempo entre disparos). </li>
			<li><strong>(weaponName)+MaxAmmo:</strong> M�xima cantidad de munici�n equipable. </li>
			<li><strong>(weaponName)+Id:</strong> Identificador del arma. </li>
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
		virtual bool accept(CMessage *message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		//__________________________________________________________________

		/**
		M�todo llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================

		/**
		Este es el m�todo que todas las armas deben redefinir. Es el que implementa
		la acci�n de disparar.
		*/
		virtual void shoot();

		//__________________________________________________________________

		/** 
		Activa el componente. Lo utilizaremos para activar el componente del arma
		que est� en uso.
		*/
		virtual void activate();

		//__________________________________________________________________

		/** 
		A�ade munici�n a un arma concreta.

		@param weapon N�mero de arma a la que queremos a�adir munici�n.
		@param ammo Cantidad de munici�n que vamos a a�adir.
		@param iAmCatch true si tengo el arma de la munici�n que he recogido.
		*/
		virtual void addAmmo(int weapon, int ammo, bool iAmCatch);

		//__________________________________________________________________

		/**
		M�todo que decrementa la cantidad de munici�n que tenemos. Debido a que
		no todas las armas decrementan su munici�n de la misma forma (algunas
		ni siquiera tienen munici�n) este m�todo es virtual.
		*/
		virtual void decrementAmmo();

		//__________________________________________________________________

		/** M�todo est�tico que resetea la cantidad de munici�n del arma. */
		void resetAmmo();

	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/** 
		Dibuja un rayo en Ogre. �til para debuggear las armas que disparan mediante
		raycast.
		*/
		void drawRaycast(const Ray& raycast);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Nombre del arma. */
		std::string _name;

		/** Altura de disparo. */
		int _heightShoot;

		/** Da�o del arma. */
		unsigned char _damage;

		/** Da�o que el arma refleja. */
		unsigned char _damageReflect;

		/** Dispersi�n del arma. */
		float _dispersion;

		/** Distancia de alcance del arma. */
		float _distance;

		/** N�mero de balas por disparo. */
		unsigned char _numberShoots;

		/** Munici�n m�xima que puede tener el arma. -1 si no tienes el arma. */
		int _maxAmmo;

		/** Munici�n actual. */
		int _currentAmmo;

		/** Identificador del arma. */
		int _id;

		/** Nombre del arma. */
		std::string _nameWeapon;
		
		/** Radio de la capsula leido del mapa. */
		float _capsuleRadius;

		/**
		Para comprobar que un arma esta disparando y no puede disparar 
		"tan rapido como puedas arreglar el gatillo" (Marcus, Borderlands 2)
		*/
		bool _canShoot;
		
		/** Timer para el cooldown. */
		int _coldDownTime;

		/** Cooldown del arma (tiempo entre disparo y disparo). */
		unsigned char _coldDown;

		/** Variable de debug, se usa para darle un nombre unico a cada raycast. */
		int _temporal;
	}; // class CShoot

	REG_FACTORY(CShoot);

} // namespace Logic

#endif // __Logic_Shoot_H
