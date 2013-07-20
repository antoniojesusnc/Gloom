//---------------------------------------------------------------------------
// Sniper.h
//---------------------------------------------------------------------------

/**
@file Sniper.h

Contiene la declaraci�n del componente que implementa el arma sniper.

@author Antonio Jesus Narv�ez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_Sniper_H
#define __Logic_Sniper_H

#include "Logic/Entity/Components/Weapon.h"
#include "Physics/RaycastHit.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementaci�n por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narv�ez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CSniper : public IWeapon {
		DEC_FACTORY(CSniper);

	public:

		/** Constructor por defecto. */
		CSniper() : IWeapon("sniper"), _primaryFireTimer(0), _secondaryFireTimer(0), _primaryFireIsActive(false), _secondaryFireIsActive(false), _burned(false) {}

		virtual ~CSniper();

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		
		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		
		/**
		Llamado cuando el arma pasa a ser activa.
		*/
		virtual void onActivate();

		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		M�todo que se encarga de realizar el disparo primario del raycast por fisicas.
		Tambi�n aplicar� el da�o.

		*/
		virtual void primaryFire();


		/**
		M�todo que se encarga de realizar el disparo secundario del raycast por fisicas.
		Tambi�n aplicar� el da�o.
		*/
		virtual void secondaryFire();


		/**
		M�todo que se encarga de intentar aplicar da�o por expansi�n al utilizar el disparo secundario de la sniper.

		@param entityHit Entidad a partir de la cual queremos expandir el da�o.
		@return Devuelve la entidad a la que hay que aplicar da�o de expansi�n.
		*/
		CEntity* findEnemyToExpand(CEntity* entityHit);

		/**
		M�todo que se encarga de mandar los mensajes que correspondan a la entidad
		que se ha golpeado en caso de hacer hit.

		A pesar de que se trata de un m�todo virtual puro, esta clase ofrece una
		implementaci�n por defecto que ser� �til para la mayor�a de las subclases. 

		@param entityHit puntero a la entidad que se ha dado (o NULL si no se ha colisionado con ninguna).
		*/
		void triggerHitMessages(CEntity* entityHit, float damageFire);

		virtual void amplifyDamage(unsigned int percentage);

		/**
		Dibuja el rayo de disparo de la sniper
		*/
		void drawRaycast(const Ray& raycast, int distance);

	private:

		//void onTick(unsigned int msecs);

		/**
		Distancia m�xima de expansi�n para el disparo secundario de la sniper.
		*/
		float _maxExpansiveDistance;

		/**
		Numero de balas que consume el disparo secundario.
		*/
		int _secondaryConsumeAmmo;
		
		/**
		Distancia
		*/
		int _shotsDistance;

		int  _defaultPrimaryFireCooldown;
		int _primaryFireCooldown;

		float _defaultPrimaryFireDamage;
		float _primaryFireDamage;

		int _defaultSecondaryFireCooldown;
		int _secondaryFireCooldown;

		float _defaultSecondaryFireDamage;
		float _secondaryFireDamage;

		int _primaryFireTimer;
		int _secondaryFireTimer;

		bool _primaryFireIsActive;
		bool _secondaryFireIsActive;

		/**
		Manejo de incremento de da�o por atravesar una bola de fuego.
		*/
		float _burnedIncrementPercentageDamage;
		bool _burned;
		

	}; // class CSniper

	REG_FACTORY(CSniper);

} // namespace Logic

#endif // __Logic__H
