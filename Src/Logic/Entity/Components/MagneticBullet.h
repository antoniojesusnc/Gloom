/**
@file MagneticBullet.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CMagneticBullet
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Mayo, 2013
*/
#ifndef __Logic_MagneticBullet_H
#define __Logic_MagneticBullet_H

#include "Logic/Entity/Component.h"


namespace Logic {
	class CShootShotGun;

}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la luz que tiene un objeto. 
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
*/
	class CMagneticBullet : public IComponent
	{
		DEC_FACTORY(CMagneticBullet);
	public:
		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CMagneticBullet() : IComponent() {}

		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		mensajes aceptados por el componente
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		void setOwner(CShootShotGun *owner);

		void impact(CEntity *impactEntity);
	protected:

		/**
		M�todo llamado en cada frame que actualiza el estado del componente de luz.
				

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		//virtual void onTick(unsigned int msecs);
		CShootShotGun *_owner;

	}; // class CMagneticBullet

	REG_FACTORY(CMagneticBullet);

} // namespace Logic

#endif // __Logic_MagneticBullet_H
