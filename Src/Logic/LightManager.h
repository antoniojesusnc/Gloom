//---------------------------------------------------------------------------
// LightManager.h
//---------------------------------------------------------------------------

/**
@file LightManager.h

Contiene la declaraci�n del manager de luces.

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
	Implementa el manager de luces. A pesar de que a trav�s de la
	atenuaci�n es posible conseguir que Ogre no renderice las luces
	cuando no estan dentro del frustrum de c�mara el manager de
	luces sirve para limitar el n�mero de luces activas en el 
	juego.

	@author Francisco Aisa Gac�a
	@date Julio, 2013
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


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Crea una luz.

		@param light Componente grafico que encapsula la luz. Una vez creada la luz
		el manager devuelve un puntero a la nueva luz.
		@param lightType Tipo de luz que queremos crear, puntual, focal o direccional.
		@param lightGroup Grupo de la luz, usado para comparar con la mascara de luz
		de las entidades graficas (para verificar si la luz afecta o no).
		@param isStatic true si la luz es estatica. Las luces estaticas procesan el
		shader de bump a partir de una textura precocinada, por lo que practicamente
		no realizan calculos. Si la luz es dinamica se realiza el calculo completo
		de bump.
		@param lightComp Componente de luz asociado a la luz gr�fica. Se usa para
		notificarle cuando la luz sea destruida por el manager (si se le cede
		el control a manager).
		@param controlledByManager true si queremos que el manager tenga en cuenta 
		esta luz. Cuando el n�mero de m�ximo de luces soportadas se alcance, el
		manager empezara a apagar las luces m�s antiguas.
		@param position Posici�n en la que queremos crear la luz.
		@param direction Direcci�n a la que queremos que apunte la luz. En el caso
		de ser puntual, no afecta la direcci�n que pongamos.
		*/
		void createLight(Graphics::CLight* & light, Graphics::LightType::Enum lightType, unsigned int lightGroup, 
						 bool isStatic, Logic::CLight* lightComp, bool controlledByManager, const Vector3& position, const Vector3& direction);

		//________________________________________________________________________

		/**
		Destruye una luz. Este m�todo podr�a haber sido implementado directamente
		por el componente l�gico que encapsula a la luz, sin embargo, he preferido
		dejarlo en el manager de luces para que la creaci�n y destrucci�n de todas
		las luces se gestione en el mismo sitio.
		*/
		void destroyLight(Graphics::CLight* light, bool controlledByManager);


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


		/** N�mero m�ximo de luces que el gestor de luces permite que haya por escena. */
		int MAX_LIGHTS;

		/** Lista que contiene parejas de luz - componente l�gico de luz */
		std::list< std::pair<Graphics::CLight*, Logic::CLight*> > _activeLights;

		/** �nica instancia de la clase. */
		static CLightManager* _instance;
	};

};

#endif