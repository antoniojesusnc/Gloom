/**
@file PlayerInterpolator.h


@see Logic::CPlayerInterpolator
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_PlayerInterpolator_H
#define __Logic_PlayerInterpolator_H

#include "Logic/Entity/Component.h"
#include "Logic/Messages/MessagePlayerSnapshot.h"

#include <deque>

namespace Logic {
	class CPhysicController;
}

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/
	
	class CPlayerInterpolator : public IComponent {
		DEC_FACTORY(CPlayerInterpolator);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CPlayerInterpolator();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CPlayerInterpolator();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>interpolationTimestep:</strong> Tiempo de interpolaci�n. </li>
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
			<li></li>
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
		//                            METODOS PROPIOS
		// =======================================================================


	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		virtual void onStart();


		/**
		M�todo llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		par�metros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onFixedTick(unsigned int msecs);


	private:


		void interpolateSnapshot(const std::vector<Matrix4>& buffer);

		void storeSnapshot(const std::shared_ptr<CMessagePlayerSnapshot>& snapshotMsg);

		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================
		
		unsigned int _ticksPerSample;
		unsigned int _samplesPerSnapshot;
		
		unsigned int _ticksPerBuffer;
		unsigned int _tickCounter;

		unsigned int _lostTicks;

		std::deque<Matrix4> _transformBuffer;
		std::deque<AnimInfo> _animationBuffer;
		std::deque<AudioInfo> _audioBuffer;
		CPhysicController* _controller;

		bool _connecting;

		unsigned int _extrapolatedTicks;
		Vector3 _extrapolatedMotion;
	}; // class CPlayerInterpolator

	REG_FACTORY(CPlayerInterpolator);

} // namespace Logic

#endif // __Logic_TransformInterpolator_H
