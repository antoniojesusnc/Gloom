//---------------------------------------------------------------------------
// Screamer.h
//---------------------------------------------------------------------------

/**
@file Screamer.h

Contiene la declaraci�n de la clase que
implementa las habilidades del personaje
"Screamer".

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_Screamer_H
#define __Logic_Screamer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaraci�n de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer".

	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CScreamer : public CPlayerClass {
		DEC_FACTORY(CScreamer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamer();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamer();
		

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
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void onActivate();

		
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


	protected:

		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          M�TODOS PRIVADOS
		// =======================================================================


		void blowUp();

		//__________________________________________________________________

		/**
		Crea una explosion mediante overlap.
		*/
		void createExplotion();


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
		unsigned int _screamerShieldDamageTimeStep;

		/** Cantidad de da�o que se va restando al escudo de la habilidad primaria con el tiempo. */
		float _screamerShieldDamageOverTime;

		/** Timer para controlar cuando hay que restar energia al escudo del screamer. */
		unsigned int _screamerShieldDamageTimer;

		/** Cada cuanto se recupera el escudo del screamer si no esta usando la habilidad primaria. */
		unsigned int _screamerShieldRecoveryTimeStep;

		/** Cuantos puntos de escudo recupera el screamer con el tiempo. */
		float _screamerShieldRecoveryOverTime;

		/** Cronometro para calcular cada cuanto hay que subirle los puntos de escudo al Screamer. */
		unsigned int _screamerShieldRecoveryTimer;

		/** Da�o de la explosi�n que provoca el screamer al soportar m�s da�os de la cuenta con el escudo. */
		float _screamerExplotionDamage;

		/** Radio de la explosi�n si el Screamer explota. */
		float _screamerExplotionRadius;

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

		/**
		Nombre del material original de la entidad
		*/
		std::string _materialName;

	}; // class CScreamer

	REG_FACTORY(CScreamer);

} // namespace Logic

#endif // __Logic_Screamer_H
