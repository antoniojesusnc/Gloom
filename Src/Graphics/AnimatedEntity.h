//---------------------------------------------------------------------------
// AnimatedEntity.h
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author Rub�n Mulero
@date June, 2013
*/

#ifndef __Graphics_AnimatedEntity_H
#define __Graphics_AnimatedEntity_H

#include "Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class AnimationState;
}

namespace Graphics 
{
	class CScene;
	class CEntity;
}
	
namespace Graphics 
{
	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en una entidad animada para enterarse de cuando
	terminan las animaciones de �sta.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CAnimatedEntityListener 
	{
	public:

		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		virtual void animationFinished(const std::string &animation) {}

	}; // CAnimatedEntityListener

	/**
	Clase que representa una entidad gr�fica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. A�ade la posibilidad de activar una animaci�n 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animaci�n activa.
	Podr�a ser m�s sofisticada permitiendo interpolaci�n de animaciones o avisando
	mediante observers cuando una animaci�n termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gr�fico.
	
	@ingroup graphicsGroup

	@author Rub�n Mulero
	@date June, 2013
	*/
	class CAnimatedEntity : public CEntity
	{

		enum AnimState{
			FADE_IN,
			FADE_OUT,
			RUNNING
		};

		struct Animation{
			AnimState state;
			Ogre::AnimationState* animation;
			float fadeTime;
		};

	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CAnimatedEntity(const std::string &name, const std::string &mesh):
					CEntity(name,mesh), _weapon(0), _graphicsWeapon(0) {}

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CAnimatedEntity() {}

		/**
		Activa una animaci�n a partir de su nombre.

		@param anim Nombre de la animaci�n a activar.
		@param loop true si la animaci�n debe reproducirse c�clicamente.
		@param fadeTime El tiempo de fade-in fade-out de la animaci�n

		@return true si la animaci�n solicitada fue correctamente activada.
		*/
		bool setAnimation(const std::string &anim, bool loop, float fadeTime = 0.15f);

		/**
		Le pone un arma en la mano al monigote

		
		*/
		void attachWeapon(CEntity &arma, unsigned int id);
		
		/**
		Desactiva una animaci�n a partir de su nombre.

		@param anim Nombre de la animaci�n a activar.
		@return true si la animaci�n solicitada fue correctamente desactivada.
		*/
		bool stopAnimation(const std::string &anim);
		
		/**
		Desactiva todas las animaciones de una entidad.
		*/
		void stopAllAnimations();

		/**
		Funci�n que registra al oyente de la entidad gr�fica. Por 
		simplicidad solo habr� un oyente por entidad.
		*/
		void addObserver(CAnimatedEntityListener *observer){
			_observers.push_back(observer);
		}

		/**
		Funci�n que quita al oyente de la entidad gr�fica. Por 
		simplicidad solo habr� un oyente por entidad.
		*/
		void removeObserver(CAnimatedEntityListener *observer){_observers.remove(observer);}

		void changeMaterialToWeapon(const std::string& materialName);

		std::string getWeaponMaterial();

		/**
		Orienta el bone en la orientaci�n que se le pasa como par�metro

		@param bone El hueso que estamos modificando
		@param orientation la orientaci�n que le estamos aplicando al hueso
		*/
		void moveBone(const std::string &bone, float pitch);

		/**
		Hace que la orientaci�n del hueso no est� atachada a la orientaci�n del padre

		@param bone El hueso que queremos liberar
		*/
		void freeBoneOrientation(const std::string &bone);

		/**
		Hace que la orientaci�n del hueso est� atachada a la orientaci�n del padre.

		@param bone El hueso que atachar.
		*/
		void lockBoneOrientation(const std::string &bone);

	protected:

		/**
		Objeto oyente que es informado de cambios en la entidad como 
		la terminaci�n de las animaciones. Por simplicidad solo habr�
		un oyente por entidad.
		*/
		std::list<CAnimatedEntityListener*> _observers;

		// Cada entidad debe pertenecer a una escena. Solo permitimos
		// a la escena actualizar el estado.
		friend class CScene;
		
		/**
		Actualiza el estado de la entidad cada ciclo.
		
		@param secs N�mero de milisegundos transcurridos desde la �ltima 
		llamada.
		*/
		virtual void tick(float secs);

		Ogre::Entity *_weapon;

		Graphics::CEntity *_graphicsWeapon;

		Ogre::SceneNode* _ObjectentityNode;

		/**
		Estructura de datos de las animaciones que hay ejecutandose
		*/
		std::map< std::string, Animation > _runningAnimations;

		typedef std::pair<std::string, Animation> TAnim;

		/**
		lista de animaciones que deben ser eliminadas de nuestra
		lista de animaciones ejecutandose
		*/
		std::list<std::string> _deletedAnims;
	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
