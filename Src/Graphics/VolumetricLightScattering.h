//---------------------------------------------------------------------------
// VolumetricLightScattering.h
//---------------------------------------------------------------------------

/**
@file VolumetricLightScattering.h

@author Francisco Aisa Garc�a
@date Agosto, 2013
*/

#ifndef __Graphics_VolumetricLightScattering_H
#define __Graphics_VolumetricLightScattering_H

#include <OgreMatrix4.h>
#include <OgreCamera.h>
#include <OgreCompositorInstance.h>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre {
	class CompositorManager;
}

namespace Graphics {
	class CCamera;
}

/**

@author Francisco Aisa Garc�a
@date Agosto, 2013
*/
namespace Graphics {
	
	/**
	Clase para administrar las variables pasadas a los compositors.

	@author Francisco Aisa Garc�a
	@date Agosto, 2013
	*/
	
	class CVolumetricLightScattering : public Ogre::CompositorInstance::Listener {
	public:

		CVolumetricLightScattering(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera);
		~CVolumetricLightScattering();

		virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

	protected:
		
		Ogre::CompositorInstance* _compositor;
		Ogre::Camera* _sceneCamera;
		Ogre::SceneNode* _cameraNode;
	};

} // namespace Graphics

#endif // __Graphics_MotionBlur_H
