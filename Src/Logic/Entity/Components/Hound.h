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

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>TOUCHED</li>
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

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void onActivate();

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Beserker, el personaje dispara m�s r�pido y hace m�s da�o. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

		/**
		Se dispara cuando se deja de pulsar la tecla que dispara la habilidad primaria.
		Notar que este m�todo no se ha hecho virtual puro (abstracto) porque muchas
		de las habilidades no necesitar�n tenerlo en cuenta. No obstante, dado que
		el mensaje que se recibe para informarnos de esta acci�n es un mensaje de
		control, he decidido tenerlo en cuenta en la clase padre.

		Para que el uso de este m�todo tenga sentido lo normal es que el cooldown
		de la habilidad sea 0.
		*/
		virtual void stopPrimarySkill();

		/** Metodo a invocar cuando haya pasado el tiempo de la secondary skill*/
		virtual void stopSecondarySkill();

	protected:

		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		//virtual void onFixedTick(unsigned int msecs);

		virtual void onTick(unsigned int msecs);

		virtual void onStart(unsigned int msecs);

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

		CEntity* _biteTrigger;

		CPhysicDynamicEntity* _trigger;
		CAvatarController* _avatarController;
		CPhysicController* _physicController;
	}; // class CHound

	REG_FACTORY(CHound);

} // namespace Logic

#endif // __Logic_Hound_H
