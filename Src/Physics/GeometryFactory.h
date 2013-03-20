/**
@file EntityFactory.h

Contiene la declaraci�n de la clase factor�a de entidades
del juego.

@see Logic::CEntityFactory

@author David Llans� Garc�a, Marco Antonio G�mez Mart�n
@date Agosto, 2010
*/

#ifndef __Physics_GeometryFactory_H
#define __Physics_GeometryFactory_H

#include "BaseSubsystems/Math.h"

#include "geometry/PxSphereGeometry.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxCapsuleGeometry.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

// Definici�n de la clase
namespace Physics {

	typedef physx::PxGeometry Geometry;
	typedef physx::PxBoxGeometry BoxGeometry;
	typedef physx::PxSphereGeometry SphereGeometry;
	typedef physx::PxCapsuleGeometry CapsuleGeometry;
	typedef physx::PxPlane PlaneGeometry;

	// Enumerado para indicar el tipo de geometria que queremos crear
	enum GeometryType { 
		eBOX,			// Caja
		eCAPSULE,		// Capsula
		eSPHERE,		// Esfera
		eCONVEX_MESH,	// Maya convexa
		eHEIGHT_FIELD,	// Terreno
		ePLANE,			// Plano
		eTRIANGLE_MESH	// Maya de triangulos
	};

	/**
	
	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CGeometryFactory {
	public:

		/**
		Inicializa la base de datos de la factor�a.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la base de datos. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Devuelve un puntero al �nico objeto de la clase.

		@return Factor�a de entidades.
		*/
		static CGeometryFactory *getSingletonPtr() { return _instance; }

		// Crear una esfera
		physx::PxSphereGeometry createSphere(float radius);

		// Crear una caja
		physx::PxBoxGeometry createBox(const Vector3& dimensions);

		// Crear un plano
		physx::PxPlane createPlane(const Vector3 &point, const Vector3 &normal);

	protected:

		/**
		�nica instancia de la clase.
		*/
		static CGeometryFactory *_instance;

		/** 
		Constructor de la clase, protegido, pues es un singleton.
		Registra al objeto como observer del cargador de mapas.
		*/
		CGeometryFactory();

		/**
		Destructor protegido, por ser singleton.
		*/
		~CGeometryFactory();

	}; // CGeometryFactory

} // namespace Physics

#endif // __Physics_GeometryFactory_H
