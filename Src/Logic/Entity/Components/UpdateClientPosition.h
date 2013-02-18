/**
@file UpdateClientPosition.h

Contiene la declaraci�n del componente que envia con cierta frecuencia
desde el servidor al cliente la posicion que existe en el servidor.

@author Francisco Aisa Garc�a
@date Octubre, 2010
*/
#ifndef __Logic_UpdateClientPosition_H
#define __Logic_UpdateClientPosition_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic  {

	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CUpdateClientPosition : public IComponent {
		DEC_FACTORY(CUpdateClientPosition);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CUpdateClientPosition() : IComponent(), _timer(0) {}

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajar� cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	private:
		float _timer;
		float _syncPosTimeStamp;
	}; // class CUpdateClientPosition

	REG_FACTORY(CUpdateClientPosition);

} // namespace Logic

#endif // __Logic_Life_H
