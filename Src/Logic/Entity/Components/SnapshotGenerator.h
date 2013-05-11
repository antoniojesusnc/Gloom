/**
@file SnapshotGenerator.h


@see Logic::CSnapshotGenerator
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Mayo, 2013
*/

#ifndef __Logic_SnapshotGenerator_H
#define __Logic_SnapshotGenerator_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Mayo, 2013
	*/
	
	class CSnapshotGenerator : public IComponent {
		//DEC_FACTORY(CSnapshotGenerator); No permitido por ser de tipo abstracto
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CSnapshotGenerator();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CSnapshotGenerator();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>snapshotsPerSecond:</strong> N�mero de snapshots que se mandan por segundo. </li>
			<li><strong>samplesPerSnapshot:</strong> N�mero de muestras por snapshot. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		virtual void sendSnapshot() = 0;

		//__________________________________________________________________

		virtual void takeSnapshot() = 0;

	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		M�todo llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		par�metros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onFixedTick(unsigned int msecs);


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================
		
		unsigned int _ticksPerSample;

		unsigned int _samplesPerSnapshot;

		unsigned int _ticksPerSampleCounter;

		unsigned int _samplesPerSnapshotCounter;

	}; // class CSnapshotGenerator

	//REG_FACTORY(CSnapshotGenerator);

} // namespace Logic

#endif // __Logic_SnapshotGenerator_H
