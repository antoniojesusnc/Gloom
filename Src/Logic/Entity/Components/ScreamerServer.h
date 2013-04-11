//---------------------------------------------------------------------------
// ScreamerServer.h
//---------------------------------------------------------------------------

/**
@file ScreamerServer.h

Contiene la declaraci�n de la clase que
implementa las habilidades del personaje
"Screamer" (parte del servidor).

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerServer_H
#define __Logic_ScreamerServer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaraci�n de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer" (parte del servidor).

	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CScreamerServer : public CPlayerClass {
		DEC_FACTORY(CScreamerServer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerServer();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamerServer();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>screamerShieldThreshold:</strong> Da�o m�ximo que aguanta el escudo del Screamer. </li>
			<li><strong>screamerShieldDamageTimeStep:</strong> Cada cuanto se baja el escudo cuando est� activado. </li>
			<li><strong>screamerShieldDamageOverTime:</strong> Cu�nto da�o se le quita al escudo en cada iteraci�n. </li>
			<li><strong>screamerShieldRecoveryTimeStep:</strong> Cada cuanto se recupera el escudo cuando est� en recarga. </li>
			<li><strong>screamerShieldRecoveryOverTime:</strong> Cu�nto se recupera el escudo en cada iteraci�n. </li>
			<li><strong>screamerExplotionDamage:</strong> Da�o de la explosi�n al explotar el Screamer. </li>
			<li><strong>screamerExplotionRadius:</strong> Radio de la explosi�n al explotar el Screamer. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void activate();

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** 
		Absorbe todos los proyectiles que le llegan de frente. Esta habilidad tiene
		un l�mite, si se sobrepasa explotas.
		*/
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

		//__________________________________________________________________

		/**
		Se dispara cuando el jugador deja de pulsar la tecla de uso de la habilidad
		primaria.

		Provoca que deje de usarse la habilidad primaria y comience la recuperacion
		de las defensas del Screamer.
		*/
		virtual void stopPrimarySkill();

		//__________________________________________________________________

		/**
		Resta al escudo del screamer la cantidad de puntos dados. Si absorbe demasiados
		puntos de da�o, el personaje explotar�.

		@param damage Da�o que absorbe el escudo del Screamer.
		*/
		void absorbDamage(float damage);


	private:


		// =======================================================================
		//                          M�TODOS PRIVADOS
		// =======================================================================


		/** Activa el shader del escudo y a la entidad */
		void activateScreamerShield();

		//__________________________________________________________________

		/** Desactiva el shader del escudo y a la entidad. */
		void deactivateScreamerShield();

		//__________________________________________________________________

		/**
		Crea una explosion mediante overlap.
		*/
		void createExplotion();

		//__________________________________________________________________

		/** Recoloca el escudo del screamer en la posici�n del punto de mira. */
		void refreshShieldPosition();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** true si la habilidad primaria est� siendo usada. */
		bool _primarySkillIsActive;

		/** Puntos de escudo actuales de la habilidad primaria. */
		float _currentScreamerShield;

		/** Limite de aguante del escudo generado por la habilidad primaria. */
		float _screamerShieldThreshold;

		/** Cada cuanto baja la resistencia del escudo de la habilidad primaria cuando est� activa. */
		float _screamerShieldDamageTimeStep;

		/** Cantidad de da�o que se va restando al escudo de la habilidad primaria con el tiempo. */
		float _screamerShieldDamageOverTime;

		/** Timer para controlar cuando hay que restar energia al escudo del screamer. */
		float _screamerShieldDamageTimer;

		/** Cada cuanto se recupera el escudo del screamer si no esta usando la habilidad primaria. */
		float _screamerShieldRecoveryTimeStep;

		/** Cuantos puntos de escudo recupera el screamer con el tiempo. */
		float _screamerShieldRecoveryOverTime;

		/** Cronometro para calcular cada cuanto hay que subirle los puntos de escudo al Screamer. */
		float _screamerShieldRecoveryTimer;

		/** Da�o de la explosi�n que provoca el screamer al soportar m�s da�os de la cuenta con el escudo. */
		float _screamerExplotionDamage;

		/** Radio de la explosi�n si el Screamer explota. */
		float _screamerExplotionRadius;

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

	}; // class CScreamerServer

	REG_FACTORY(CScreamerServer);

} // namespace Logic

#endif // __Logic_ScreamerServer_H
