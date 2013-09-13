//---------------------------------------------------------------------------
// Hound.h
//---------------------------------------------------------------------------

/**
@file Hound.h

Contiene la declaraci�n de la clase que
implementa las habilidades del personaje
"Hound".

@author Jose Antonio Garc�a Y��ez.
@author Rub�n Mulero Guerrero.
@date Marzo, 2013
*/

#ifndef __Logic_Hound_H
#define __Logic_Hound_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {
	//forward declarations 
	class CPhysicDynamicEntity;
	class CAvatarController;
	class CPhysicController;
	class CBite;

	/**
	Clase que implementa las habilidades propias
	del personaje "Hound".

	@ingroup logicGroup

	@author Jose Antonio Garc�a Y��ez.
	@date Marzo, 2013
	*/

	class CHound : public CPlayerClass {
		DEC_FACTORY(CHound);
	public:
		

		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CHound();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CHound();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>houndBerserkerDuration:</strong> Tiempo que dura el efecto 
			beserker desde que se activa. </li>
			<li><strong>houndBerserkerDamagePercent:</strong> Porcentaje de aumento del
			da�o. </li>
			<li><strong>houndBerserkerCooldownPercent:</strong> Porcentaje de reducci�n
			de cooldowns. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void onActivate();

		virtual void onDeactivate();
		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Beserker, el personaje dispara m�s r�pido y hace m�s da�o. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

		/** Metodo a invocar cuando haya pasado el tiempo de la secondary skill*/
		virtual void stopSecondarySkill();

	protected:

		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		//virtual void onFixedTick(unsigned int msecs);

		virtual void onTick(unsigned int msecs);

		virtual void onStart();

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Tiempo que dura el berserker */
		float _berserkerDuration;

		/** Timer que controla la duraci�n del berserker. */
		float _berserkerTimer;
		
		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingPrimarySkill;

		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingSecondarySkill;

		float _maxDefaultVelocity;

		float _bitetVelocity;

		int _biteTimer;

		float _biteDuration;

		float _biteDamage;

		float _biteMaxVelocity;

		bool charge;
		
		/**
		Nombre del material original de la entidad
		*/
		std::string _materialName;

		CEntity* _biteEntity;
		CPhysicController* _physicController;
	}; // class CHound

	REG_FACTORY(CHound);

} // namespace Logic

#endif // __Logic_Hound_H
