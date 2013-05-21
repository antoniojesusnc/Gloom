//---------------------------------------------------------------------------
// ShootRaycast.h
//---------------------------------------------------------------------------

/**
@file ShootRaycast.h

Contiene la declaraci�n del componente que abstrae la implementaci�n del 
disparo por raycast.

@author Antonio Jesus Narv�ez Corrales
@author Francisco Aisa Garc�a
@date Febrero, 2013
*/

#ifndef __Logic_ShootRaycast_H
#define __Logic_ShootRaycast_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Componente que implementa de forma general el comportamiento de aquellas
	armas que se basan en el uso de raycast. Contiene m�todos �tiles como
	el dibujado de raycast (para debuggear) y el disparo de raycast basado
	en arquetipos.

	@author Antonio Jesus Narv�ez Corrales
	@author Francisco Aisa Garc�a
	@date Febrero, 2013
	*/

	class CShootRaycast : public CShoot {
		//DEC_FACTORY(CShootRaycast); -- Esta clase es abstracta y por lo tanto no instanciable
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShootRaycast() : CShoot(), _temporal(0) { }

		virtual ~CShootRaycast();

		//__________________________________________________________________

		/**
		Constructor parametrizado

		@param shoot Nombre del arma que vamos a inicializar.
		*/
		CShootRaycast(const std::string &shoot) : CShoot(shoot), _temporal(0) { }


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicializaci�n del componente utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		Inicializaci�n del componente a partir de la informaci�n extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>(weaponName)+Dispersion:</strong> Dispersion del arma. </li>
			<li><strong>(weaponName)+Distance:</strong> Distancia que alcanza el arma. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                    METODOS HEREDADOS DE CSHOOT
		// =======================================================================


		/**
		Debido a que todos los disparos por raycast son realmente similares, este
		m�todo implementa el patr�n template, siendo �nicamente necesario redefinir
		el m�todo "triggerHitMessages".

		Notar, que la funci�n shoot no se ha hecho est�tica en CShoot porque los
		proyectiles y los raycast tienen comportamientos muy distintos.
		*/
		virtual void primaryShoot();

		/**
		Los disparos secundarios si fieren bastante unos de otros, por ahora se ha hecho un metodo vacio,
		para poder compilar por ahora, pero todos tendran que ser redefinidos.
		*/
		virtual void secondaryShoot();

		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		M�todo que se encarga de realizar el disparo del raycast por fisicas.

		Notar que este m�todo si es est�tico porque el disparo mediante raycast 
		est� totalmente parametrizado en los arquetipos, y por lo tanto, todas las
		armas pueden seguir el mismo patr�n de disparo.

		@return Un puntero a la entidad con la que se ha colisionado (o NULL si no
		ha habido colisi�n) y el rayo que se lanz� para comprobar dicha colisi�n.
		*/
		virtual std::pair<CEntity*, Ray> fireWeapon();

		//__________________________________________________________________

		/**
		M�todo que se encarga de mandar los mensajes que corresopondan a la entidad
		que se ha golpeado en caso de hacer hit.

		A pesar de que se trata de un m�todo virtual puro, esta clase ofrece una
		implementaci�n por defecto que ser� �til para la mayor�a de las subclases. 

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit) = 0;

	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/** 
		Dibuja un rayo en Ogre. �til para debuggear las armas que disparan mediante
		raycast.

		@param raycast Raycast a dibujar.
		*/
		void drawRaycast(const Ray& raycast);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Da�o que el arma refleja. */
		unsigned char _damageReflect;

		/** Dispersi�n del arma. */
		float _dispersion;

		/** Dispersi�n original del arma. Variable auxiliar para guardar la referencia le�da del mapa.*/
		float _dispersionOriginal;

		/** Distancia de alcance del arma. */
		float _distance;
		
		/** Variable de debug, se usa para darle un nombre unico a cada raycast. */
		int _temporal;

		/** Ruta del sonido de no munici�n */
		std::string _noAmmo;
		
		/** Ruta del sonido de disparo */
		std::string _audioShoot;



	};

	//REG_FACTORY(CShootRaycast);

} // namespace Logic

#endif // __Logic_Shoot_H
