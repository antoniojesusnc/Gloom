//---------------------------------------------------------------------------
// ShootProjectile.h
//---------------------------------------------------------------------------

/**
@file ShootProjectile.h

Contiene la declaraci�n del componente que abstrae la implementaci�n del 
disparo de proyectiles.

@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_ShootProjectile_H
#define __Logic_ShootProjectile_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {

	/**
	@ingroup logicGroup

	Componente que implementa de forma general el comportamiento de aquellas
	armas que disparan proyectiles.

	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CShootProjectile : public CShoot {
		//DEC_FACTORY(CShootProjectile); -- Esta clase es abstracta y por lo tanto no instanciable
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShootProjectile() : CShoot() {}

		//__________________________________________________________________

		/**
		Constructor parametrizado

		@param shoot Nombre del arma que vamos a inicializar.
		*/
		CShootProjectile(const std::string &shoot) : CShoot(shoot) {}
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		// =======================================================================
		//                    METODOS HEREDADOS DE CSHOOT
		// =======================================================================


		/**
		Debido a que todos los disparos siguen una pauta similar, este m�todo implementa
		el patr�n template, siendo �nicamente necesario redefinir el m�todo
		"fireWeapon".

		Debido a que los proyectiles disparados por las armas que derivan de este 
		componente ser�n entidades f�sicas, el env�o de mensajes ya no es necesario
		hacerlo desde aqu� (las propias entidades f�sicas se encargar�n de mandar
		los mensajes que correspondan).

		Notar, que la funci�n shoot no se ha hecho est�tica en CShoot porque los
		proyectiles y los raycast tienen comportamientos muy distintos.
		*/
		virtual void shoot();


		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/** M�todo que se encarga de realizar el disparo del proyectil. */
		virtual void fireWeapon() = 0;

	protected:


		// =======================================================================
		//                          MIEMBROS PROPIOS
		// =======================================================================


		float _projectileRadius;

		/** Ruta del sonido de no munici�n */
		std::string _noAmmo;

		/** Ruta del sonido de disparo */
		std::string _audioShoot;
	};

	//REG_FACTORY(CShootProjectile);

} // namespace Logic

#endif // __Logic_Shoot_H
