//---------------------------------------------------------------------------
// PoolParticle.h
//---------------------------------------------------------------------------

/**
@file PoolParticle.h

Contiene la declaraci�n de la clase contenedora de los elementos
de una escena.

@see Graphics::CPoolParticle

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_PoolParticle_H
#define __Graphics_PoolParticle_H

#include <list>
#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

namespace Graphics 
{
	class CParticle;
}

namespace Graphics 
{
	/**
	

	@author Antonio Jesus Narvaez Narvaez
	@date Mayo, 2013
	*/
	class CPoolParticle 
	{
	public:



		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
	protected:

		/**
		Constructor de la clase.
		*/
		CPoolParticle();

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CPoolParticle();

		/**
		Despierta la escena y crea un viewport que ocupa toda la
		pantalla.
		*/
		void activate();

		/**
		Duerme la escena y destruye su viewport para que no se siga 
		reenderizando.
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo. Llama a su vez a 
		todas las entidades.
		
		@param secs N�mero de segundos transcurridos desde la �ltima 
		llamada.
		*/
		void tick(float secs);


		struct TParticles{
			unsigned int index;
			std::vector<CParticle*> particles;
			TParticles(): index(0){};
		};

		std::map<std::string, TParticles> _particles;		

	}; // class CPoolParticle

} // namespace Graphics

#endif // __Graphics_PoolParticle_H
