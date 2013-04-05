/**
@file GeometryFactory.h

Contiene la declaraci�n de la factor�a de geometr�as
f�sicas.

@see Logic::CGeometryFactory

@author Francisco Aisa Garc�a.
@date Marzo, 2013.
*/

#ifndef __Physics_GeometryFactory_H
#define __Physics_GeometryFactory_H

#include "BaseSubsystems/Math.h"

// La factoria devuelve tipos estaticos (nada de punteros)
// por ello incluimos las cabeceras y no hacemos predeclaraciones
#include "geometry/PxSphereGeometry.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxCapsuleGeometry.h"

namespace Physics {

	// Alias para evitar acoplamiento en el proyecto de l�gica
	typedef physx::PxGeometry Geometry;
	typedef physx::PxBoxGeometry BoxGeometry;
	typedef physx::PxSphereGeometry SphereGeometry;
	typedef physx::PxCapsuleGeometry CapsuleGeometry;
	typedef physx::PxPlane PlaneGeometry;

	/**
	Factor�a de geometr�as f�sicas. Los actores est�n formados de shapes, que
	a su vez est�n formados por geometr�as. La factor�a nos da la posibilidad
	de crear actores y hacer queries especificando la geometr�a que queremos.

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CGeometryFactory {
	public:


		// =======================================================================
		//                 METODOS DE INICIALIZACION Y LIBERACION
		// =======================================================================


		/**
		Inicializa la factor�a de geometr�as.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera los recursos reservados por la factor�a.
		*/
		static void Release();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Devuelve un puntero al �nico objeto de la clase.

		@return Factor�a de entidades.
		*/
		static CGeometryFactory* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Crea una esfera dado un radio.

		@param radius Radio de la esfera que queremos crear
		@return La geometr�a de una esfera.
		*/
		SphereGeometry createSphere(float radius);

		//________________________________________________________________________

		/**
		Crea una caja dadas unas dimensiones.

		@param dimensions Anchura, altura y profundidad de la caja.
		@return La geometr�a de una caja.
		*/
		BoxGeometry createBox(const Vector3& dimensions);

		//________________________________________________________________________

		/**
		Crea la geometr�a de un plano.

		@param point Punto por el que pasa el plano.
		@param normal Normal del plano.
		@return La geometr�a de un plano.
		*/
		PlaneGeometry createPlane(const Vector3 &point, const Vector3 &normal);

	private:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase, privado, pues es un singleton. */
		CGeometryFactory();

		//________________________________________________________________________

		/** Destructor privado, por ser singleton. */
		~CGeometryFactory();

		//________________________________________________________________________

		/** �nica instancia de la clase. */
		static CGeometryFactory *_instance;

	}; // CGeometryFactory

} // namespace Physics

#endif // __Physics_GeometryFactory_H
