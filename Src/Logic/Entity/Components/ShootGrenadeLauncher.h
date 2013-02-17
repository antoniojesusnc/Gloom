/**
@file ShootGrenadeLauncher.h

Contiene la declaraci�n de la cabecera del componente
de disparo del lanzagranadas.

@see Logic::CShootGrenadeLauncher
@see Logic::IComponent

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_ShootGrenadeLauncher_H
#define __Logic_ShootGrenadeLauncher_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CShootGrenadeLauncher : public CShoot {
		DEC_FACTORY(CShootGrenadeLauncher);

	public:
		CShootGrenadeLauncher() : CShoot("GrenadeLauncher") { }
	};

	REG_FACTORY(CShootGrenadeLauncher);

} // namespace Logic

#endif // __Logic_Shoot_H
