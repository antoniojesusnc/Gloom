//---------------------------------------------------------------------------
// Fluid.h
//---------------------------------------------------------------------------

/**
@file Fluid.h

@see Graphics::CFluid

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_Fluid_H
#define __Physics_Fluid_H

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace physx {
	class PxParticleFluid;
	class PxScene;
	class PxPhysics;
};

namespace Physics {
	/**
	
	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/
	class CFluid {
	public:

		/** Constructor por defecto. */
		CFluid();

		/** Destructor de la aplicaci�n. */
		virtual ~CFluid();

		/**
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		//bool attachToScene(CScene *scene);

	protected:

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y elimin� de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		physx::PxParticleFluid* createFluid(unsigned int maxParticles, float restitution, float viscosity,
								            float stiffness, float dynamicFriction, float particleDistance);

		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		//Ogre::SceneNode *_entityNode;

		/** Sistema de fluidos de PhysX. */
		physx::PxParticleFluid* _fluid;

		/** @deprecated De momento tenemos la escena de physx, en un futuro sera CScene. */
		physx::PxScene* _scene;

		// SDK de Physx
		/** 
		@deprecated De momento mantenemos el puntero a la SDK, pero en el futuro se deberia
		llamar al server para que haga lo que tenga que hacer.
		*/
		physx::PxPhysics* _physics;

		/**
		Indica si la entidad ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;

		bool _runOnGPU;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
