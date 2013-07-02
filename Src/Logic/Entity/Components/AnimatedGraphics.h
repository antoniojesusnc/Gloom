/**
@file AnimatedGraphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de una entidad est�tica.

@see Logic::CAnimatedGraphics
@see Logic::CGraphics

@author Rub�n Mulero Guerrero
@author Antonio Jes�s Narv�ez Corrales

@date March, 2013
*/
#ifndef __Logic_AnimatedGraphics_H
#define __Logic_AnimatedGraphics_H

#include "Graphics.h"
#include "Graphics/AnimatedEntity.h"
#include "Graphics/Bone.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CAnimatedEntity;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica animada de una 
	entidad. Especializa a la clase CGraphics para que la entidad gr�fica
	creada sea animada.
	<p>
	Adem�s de los mensajes de cambio de posici�n y de cambio de orientaci�n
	que acepta CGraphics, acepta mensajes para poner o para una animaci�n
	(SET_ANIMATION y STOP_ANIMATION).
	
    @ingroup logicGroup

	@author Rub�n Mulero Guerrero
	@author Antonio Jes�s Narv�ez Corrales

	@date March, 2013
*/
	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:
		
		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(), _animatedGraphicsEntity(0), _currentWeapon(0), _currentMaterialWeapon("original"),
				_defaultAnimation(""), _weapons(NULL), _pi(Ogre::Radian(Math::PI),Vector3::UNIT_Y) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica y la animada.
		*/
		virtual ~CAnimatedGraphics();
		
		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_ANIMATION y STOP_ANIMATION.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);
		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		M�todo que desactiva el componente.

		@return true si todo ha ido correctamente.
		*/
		virtual void onDeactivate();
		
		// =======================================================================
		//             M�TODOS HEREDADOS DE CANIMATEDENTITYLISTENER
		// =======================================================================
		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::string &animation);

		// =======================================================================
		//                           M�TODOS PROPIOS
		// =======================================================================

		void changeWeapon(int newWeapon);

		virtual void changeMaterial(const std::string& materialName);

		Graphics::CAnimatedEntity * getAnimatedEntity(){return _animatedGraphicsEntity;}

		Graphics::CBone getBone(const std::string& boneName) const;

	protected:

		/**
		M�todo virtual que construye la entidad gr�fica animada de la entidad. 
		Sobreescribe el m�todo de la clase CGraphics.
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);
		
		virtual void onTick(unsigned int msecs);

	private:

		/**
		estructura de la siguiente animacion a ejecutar
		*/
		struct Animation{
			std::string animation;
			bool loop;
			bool exclude;
		};

		/**
		Entidad gr�fica animada.
		*/
		Graphics::CAnimatedEntity *_animatedGraphicsEntity;

		/**
		Entidades gr�ficas de las armas.
		*/
		Graphics::CEntity **_weapons;

		/**
		Animaci�n por defecto de una entidad gr�fica animada.
		*/
		std::string _defaultAnimation;

		std::string _originalMaterialWeapon;

		int _currentWeapon;

		std::string _currentMaterialWeapon;

		bool _insertAnimation;

		Animation nextAnim;

		Quaternion _pi;

	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
