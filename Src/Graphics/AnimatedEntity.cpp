//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementaci�n de la clase que representa una entidad gr�fica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
*/

#include "AnimatedEntity.h"

#include <assert.h>
#include "Server.h"
#include "Scene.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, bool loop)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		_currentAnimation = _entity->getAnimationState(anim);
		_currentAnimation->setEnabled(true);
		_currentAnimation->setLoop(loop);
		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		Ogre::AnimationState *animation = _entity->getAnimationState(anim);
		animation->setEnabled(false);
		//Por si la animacion es finita y ha acabado.
		if( animation->hasEnded() )			
			animation->setTimePosition(0);
		// Si la animaci�n a parar es la animaci�n activa ya no lo estar�.
		if(animation == _currentAnimation)
			_currentAnimation = 0;
		return true;

	} // stopAnimation

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		if(_entity->getAllAnimationStates()->hasEnabledAnimationState())
		{
			Ogre::ConstEnabledAnimationStateIterator it = 
				_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
			Ogre::AnimationState *animation;

			//hay que recorrer con el iterador las animaciones una a una y pausarlas
			while(it.hasMoreElements())
			{
				animation = it.getNext();
				animation->setEnabled(false);
				//Por si la animacion es finita y ha acabado.
				if( animation->hasEnded() )			
					animation->setTimePosition(0);
			}

			// Si hab�a animaci�n activa ya no lo est�.
			_currentAnimation = 0;
		}

	} // stopAllAnimations

	//--------------------------------------------------------
		
	void CAnimatedEntity::tick(float secs)
	{
		if(_currentAnimation)
		{
			_currentAnimation->addTime(secs);
			// Comprobamos si la animaci�n ha terminado para avisar, salvo que se trate de una animaci�n final ( en este caso muerte)
			if(_observer && _currentAnimation->hasEnded() && _currentAnimation->getAnimationName().compare("Death")!=0){
				_observer->animationFinished(_currentAnimation->getAnimationName());
			}
		}

	} // tick
	//--------------------------------------------------------

	void CAnimatedEntity::attachWeapon(CEntity &arma, unsigned int id){
		
		if(_weapon)
			_scene->getSceneMgr()->destroyEntity(_weapon);
		Ogre::String asd = arma.getMesh();
		_weapon = _scene->getSceneMgr()->createEntity("weapon"+id, arma.getMesh());
		_entity->attachObjectToBone("Bip01 R Hand",_weapon);

	}


} // namespace Graphics
