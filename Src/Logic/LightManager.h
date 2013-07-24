//---------------------------------------------------------------------------
// LightManager.h
//---------------------------------------------------------------------------

/**
@file LightManager.h

@see Logic::CLightManager

@author Francisco Aisa Garc�a
@date Julio, 2013
*/

#ifndef __Logic_LightManager_H
#define __Logic_LightManager_H

#include "Graphics/Light.h"
#include <deque>

// Predeclaracion del typedef NetID
namespace Logic {

	class CLight;

	/**

	*/

	class CLightManager {
	public:

		

		// =======================================================================
		//                  METODOS DE INICIALIZACION Y ACTIVACION
		// =======================================================================


		/**
		Devuelve la �nica instancia de la clase CLightManager.
		
		@return �nica instancia de la clase CLightManager.
		*/
		static CLightManager* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera la instancia de CLightManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		//________________________________________________________________________

		/** Funci�n llamada para activar la escucha. */
		void activate();

		//________________________________________________________________________

		/** Funci�n llamada desactivar la escucha. */
		void deactivate();

		Graphics::CLight* createLight(Graphics::LightType::Enum lightType, const std::string& lightName, bool isStatic, const Vector3& position, const Vector3& direction);

		void destroyLight(Graphics::CLight* light, bool isStatic);

	private:


		// =======================================================================
		//                          METODOS PRIVADOS
		// =======================================================================
		

		/** Constructor por defecto. Privado por ser la clase singleton. */
		CLightManager();

		/** Destructor. */
		~CLightManager();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================

		int MAX_LIGHTS;

		std::deque<Graphics::CLight*> _activeLights;

		/** �nica instancia de la clase. */
		static CLightManager* _instance;
	};

};

#endif