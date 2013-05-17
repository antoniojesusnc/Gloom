//---------------------------------------------------------------------------
// PlayerClass.h
//---------------------------------------------------------------------------

/**
@file PlayerClass.h

Contiene la declaraci�n del componente que define
el comportamiento general de las habilidades de cada
clase.

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Logic_PlayerClass_H
#define __Logic_PlayerClass_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"

#include <string>

namespace Logic {

	/**
	Componente encargado de la gesti�n general del uso
	de las habilidades de los personajes del juego.

	Cada clase tiene una serie de habilidades activas y
	pasivas. Las habilidades activas est�n compuestas 
	en realidad, por un ataque primario y otro secundario.

	Cada clase del juego estar� representada por un componente.
	Dicho componente deber� heredar de esta clase e implementar
	como m�nimo los m�todos que se encargan de disparar
	las habilidades primaria y secundaria.

	En principio deber�a bastar con redefinir los m�todos
	comentados anteriormente, ya que la clase base (�sta)
	se encarga de aceptar y procesar los mensajes de cambio
	de clase y de uso de las habilidades.

	IMPORTANTE: Este componente solo controla el uso de las
	habilidades activas. Tambi�n es necesario controlar
	de alguna manera las habilidades pasivas al cambiar
	de clase. HAY QUE IMPLEMENTARLO.

	@ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CPlayerClass : public IComponent {
		//DEC_FACTORY(CPlayerClass); -- Esta clase es abstracta y por lo tanto no instanciable
	public:

		// Tipo de la clase
		enum Type {
			eHOUND,
			eSCREAMER,
			eARCHANGEL,
			eSHADOW
		};

		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** 
		Constructor parametrizado. Todos los componentes necesitan un constructor
		por defecto para poder ser llamados desde la factor�a, pero dado que este
		componente es abstracto, no tenemos ese problema.

		Pasamos el nombre de la clase hija como truco para leer los datos de cooldown.
		
		@param playerClassName Nombre de la clase de personaje. Las clases que hereden
		de �sta, deben rellenar este campo (Hound, Screamer, Shadow, Archangel).
		*/
		CPlayerClass(const std::string& playerClassName);

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CPlayerClass();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>primarySkillCooldown:</strong> Tiempo de recarga de la habilidad primaria. </li>
			<li><strong>secondarySkillCooldown:</strong> Tiempo de recarga de la habilidad secundaria. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>USE_PRIMARY_SKILL</li>
			<li>USE_SECONDARY_SKILL</li>
			<li>STOP_PRIMARY_SKILL</li>
			<li>STOP_SECONDARY_SKILL</li>
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

		//__________________________________________________________________

		/**
		Extiende el comportamiento de la clase padre ya que cuando se activa
		este componente puede ser porque el jugador haya cambiado de clase
		por lo que los timers de los cooldowns tienen que volver a 0.

		Adem�s, cuando el personaje muere tambien se reactiva este componente
		por lo que las clases hijas deber�n sobreescribir este m�todo para
		resetear todos sus campos.
		*/
		virtual void onActivate();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		M�todo abstracto que debe implementar el disparo de la habilidad primaria
		del personaje.
		*/
		virtual void primarySkill() = 0;

		//__________________________________________________________________

		/**
		M�todo abstracto que debe implementar el disparo de la habilidad secundaria
		del personaje.
		*/
		virtual void secondarySkill() = 0;

		//__________________________________________________________________

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

		//__________________________________________________________________

		/**
		Se dispara cuando se deja de pulsar la tecla que dispara la habilidad primaria.
		Notar que este m�todo no se ha hecho virtual puro (abstracto) porque muchas
		de las habilidades no necesitar�n tenerlo en cuenta. No obstante, dado que
		el mensaje que se recibe para informarnos de esta acci�n es un mensaje de
		control, he decidido tenerlo en cuenta en la clase padre.

		Para que el uso de este m�todo tenga sentido lo normal es que el cooldown
		de la habilidad sea 0.
		*/
		virtual void stopSecondarySkill();


	protected:


		// =======================================================================
		//                          M�TODOS PROTEGIDOS
		// =======================================================================


		/**
		M�todo llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		//__________________________________________________________________

		/** 
		Indica si la habilidad primaria puede usarse.

		@return True si ha finalizado el cooldown de la habilidad primaria.
		*/
		inline bool canUsePrimarySkill();

		//__________________________________________________________________

		/**
		Indica si la habilidad secundaria puede usarse.

		@return True si ha finalizado el cooldown de la habilidad secundaria.
		*/
		inline bool canUseSecondarySkill();


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Tiempo de recarga de la habilidad primaria del personaje. */
		float _primarySkillCooldown;

		/** Tiempo de recarga de la habilidad secundaria del personaje. */
		float _secondarySkillCooldown;

		/** Cronometro para el cooldown de la primera skill. */
		float _primarySkillTimer;

		/** Cronometro para el cooldown de la segunda skill. */
		float _secondarySkillTimer;

		/** Altura a la que se encuentra el punto de disparo. */
		// static??
		float _heightShoot;

		/** Radio de la c�psula del jugador. */
		float _capsuleRadius;

	}; // class CPlayerClass

	//REG_FACTORY(CPlayerClass);

} // namespace Logic

#endif // __Logic_PlayerClass_H
