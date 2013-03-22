/**
@file MaterialManager.h

Contiene la declaraci�n del manager de materiales f�sicos.

@see Physics::CMaterialManager

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_MaterialManager_H
#define __Physics_MaterialManager_H

#include <vector>

// Predeclaraci�n de clases
namespace physx {
	class PxMaterial;
	class PxPhysics;
}

namespace Physics {

	// Alias para evitar acoplamiento en el proyecto de l�gica
	typedef physx::PxMaterial Material;

	// Enumerado que contiene la lista de materiales que podemos usar
	enum MaterialType {
		eDEFAULT
	};


	/**
	Gestor que mantiene una tabla de materiales f�sicos que iremos asignando
	a los actores f�sicos que creemos. La implementaci�n es pr�cticamente
	dummy, ya que probablemente no necesitaremos muchos materiales (por
	lo que un vector indexado mediante enumerados es m�s que suficiente).

	@ingroup physicGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CMaterialManager {
	public:


		// =======================================================================
		//                 METODOS DE INICIALIZACION Y LIBERACION
		// =======================================================================


		/**
		Inicializa el manager.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/** Libera los recursos reservados por el manager. */
		static void Release();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Devuelve un puntero al �nico objeto de la clase.

		@return Manager de materiales.
		*/
		static CMaterialManager* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al material f�sico que buscamos.

		@param material Tipo de material que queremos.
		*/
		Material* getMaterial(MaterialType material);

	private:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase, privado, pues es un singleton. */
		CMaterialManager();

		//________________________________________________________________________

		/** Destructor privado, por ser singleton. */
		~CMaterialManager();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** �nica instancia de la clase. */
		static CMaterialManager *_instance;

		/** Puntero a la SDK de PhysX, necesario para registrar materiales. */
		physx::PxPhysics* _physxSDK;

		/** Vector que contendr� los materiales registrados. */
		std::vector<physx::PxMaterial*> _materialTable;

	}; // CMaterialManager

}; // namespace Physics

#endif // __Physics_MaterialManager_H