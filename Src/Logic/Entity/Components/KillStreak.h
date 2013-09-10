/**
@file Elevator.h

Contiene la declaraci�n del componente que controla el componente de los kill streak

@see Logic::CKillStreak
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Agosto, 2013
*/
#ifndef __Logic_KillStreak_H
#define __Logic_KillStreak_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la gesti�n de los kill streak.
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Agosto, 2013
*/


	/* EXPLICACI�N KILL STREAKS

	===Killing Streak (V2)===

		*Hound
		x3: Aumento de al velocidad de movimiento.
		x5: Aumento del da�o y absorcion de vida del "bocao".
		x10: Aumento de la duraci�n de berserker y reducci�nd e su CD.
		x20: Aura de velocidad de movimiento. Posibilidad de "esquivar" las balas de un X% (las balas se desvian o els atraviesan sin causar da�o)

		*Archangel
		x3: Disminuci�n considerable de la reducci�n de vida base.
		x5: Aumento de la duraci�n de la inmunidad.
		x10: Aumento de la cantidad de vida que cura (a s� mismo y a los aliados). Aumento del �rea de acci�n de la curaci�n.
		x20: Zona inmune. Crea un �rea (escudo) donde no pueden atravesar los proyectiles (o ataques melee). Si pueden atravesarlo cualquier jugador (aliado o enemigo).

		*Shadow
		x3: Aumento del da�o con la melee.
		x5: Aumento de la duraci�n de la invisibilidad.
		x10: Aumento de la duraci�n de la cegera. Se incrementa el da�o que reciben los objetivos cegados.
		x20: Aura de invisiblidad. S�lo cuando un shadow es invisible, tambn lo son sus compa�eros en un peque�o radio.

		*Screamer
		x3: Aumento de la vida.
		x5: Aumento de la cantidad de da�o absorbida.
		x10: Aumento del da�o, �rea y fuerza del empuj�n.
		x20: Aura de fuerza. Los aliados cercanos ganan la habilidad de empujar en sus armas (ligeramente).

		A nivel 20 de KS se gana "inestabilidad" (de tanto consumir almas) El jugador atrae items pasivamente en un �rea. Si un jugador muere explotar� en un area grande, haciendo da�o a aliados y enemigos e incluso eliminando los items (hasta que respawneen)

		Todos los niveles de KS deber�an representarse gr�ficamente. El numero de muertes (x3, x5, x10, x20) es variable.

		===Formas inestables (V1)===
		(Representaci�n gr�fica con KS x20)

		*Hound
		Forma animal: 
		-Mas grande y musculado (solo requiere deformaci�n de la malla).
		-Peludo (cambio de textura).
		-Con cuernos/pinchos (m�s complejo, se peude hacer desde el modelo original o a�adiendole nuevas mallas que sean hijas de los huesos desde el ogre).
		-Fuego naranja de los ojos (part�culas).
		-Genera continuamente sonidos de animal/demonio.

		*Archangel
		Forma celestial:
		-Levita una animaci�n m�s para las piernas).
		-Se convierte en un ser de luz (cambio de texturas, shader glow "exagerado", halo y posiblemente part�culas) Que moleste a la vista, que casi ciegue.
		-Alas de energ�a (por aprt�culas, q es caro, o con dos planos con texturas con glow/additive).

		*Shadow
		Forma oscura:
		-Semitransparente (cambio del alfa de las texturas)
		-Humo negro girando a su alrededor (part�culas).
		-Oscurece la vision (shader) Igual que el arc�ngel te ciega con la luz, la pantalla se oscurece si se apunta directamente al shadow.

		*Screamer
		Forma ca�tica:
		-Ondas envolventes, de la concentraci�n de ondas sonoras el aire se enturbia viol�ntamente a su alrededor.
		-Venas negras por todo el cuerpo,especialmente la boca (cambiar textura).
		-Decalls de grietas y roturas a su alrededor, por las ondas.
		-Todos los jugadores pueden oir susurros, pasos y gritos continuamente (as� molesta a la percepci�n)
	*/

	class CKillStreak : public IComponent
	{
		DEC_FACTORY(CKillStreak);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CKillStreak() : IComponent(){
						_iKillsLevel1 = 3;
						_iKillsLevel2 = 5;
						_iKillsLevel3 = 10;
						_iKillsLevel4 = 20;
					}

		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void onActivate();

		/**
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick. Son siempre constantes.
		*/
		virtual void onFixedTick							(unsigned int msecs);

		//Funci�n que se llama cuando llega un mensaje de kill streak
		void dead											(const std::shared_ptr<CMessage>& message);

		//Funci�n que setea el nivel en funci�n del n�mero de muertes
		void SetLevel										();

		//Funci�n que lanza el kill streak correspondiente a la clase que sea la entidad
		void LaunchClassKillStreak							();

		void ArchangelKillStreak							();



	private:

		//Contador de muertes que lleva el jugador
		int _iContKills;		

		//Muertes l�mite que hay que hacer para subir de nivel. Se setean desde fichero
		int _iKillsLevel1, _iKillsLevel2, _iKillsLevel3, _iKillsLevel4;

		//Nivel de killstreak del jugador
		int _iLevel;

	}; // class CElevator

	REG_FACTORY(CKillStreak);

} // namespace Logic

#endif // __Logic_Elevator_H
