//---------------------------------------------------------------------------
// MutableEntity.h
//---------------------------------------------------------------------------

/**
@file MutableEntity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CMutableEntity

@author Francisco Aisa Garc�a
@date Abril, 2013
*/

#ifndef __Graphics_MutableEntity_H
#define __Graphics_MutableEntity_H

#include "BaseSubsystems/Math.h"
#include "Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n


namespace Graphics {
	
	/**
	
	@ingroup graphicsGroup

	@author Francisco Aisa Garc�a
	@date Abril, 2013
	*/

	class CMutableEntity : public CEntity {
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CMutableEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CMutableEntity();

		void getMeshInformation(const Ogre::MeshPtr& mesh,size_t &vertex_count, Ogre::Vector3* &vertices,
								size_t &index_count, unsigned* &indices,
								const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
								const Ogre::Quaternion &orient = Ogre::Quaternion::IDENTITY,
								const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
