//---------------------------------------------------------------------------
// Particle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaraci�n de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;

}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de Particle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Particle.
		*/
		CParticle(const std::string &unicName, const std::string &particleName, int contador);

		/**
		Destructor de la part�cula.
		*/
		~CParticle();


		Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}


	protected:
		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		Ogre::ParticleSystem* _particleSystem;

		/**
		Nombre del ParticleSystem.
		*/
		std::string _nameParticle;

		/**
		scene
		*/

		int _temporal;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H