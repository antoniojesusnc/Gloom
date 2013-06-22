/**
@file LifeDome.h

Contiene la declaraci�n del componente que controla la c�pula de la segunda habilidad del Archangel que da vida .

@see Logic::CLifeDome
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Mayo, 2013
*/

#ifndef __Logic_LifeDome_H
#define __Logic_LifeDome_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CArchangel;
	class CPhysicDynamicEntity;
	class CGraphics;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la c�pula que da vida del Archangel. 
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Mayo, 2013
*/
	class CLifeDome : public IComponent
	{
		DEC_FACTORY(CLifeDome);
	public:
		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLifeDome() : _scale(1.0f), IComponent(), _owner(NULL) {}

		/** Destructor. */
		virtual				~CLifeDome			();

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>SET_REDUCED_DAMAGE</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea el valor del porcentaje de da�os absorbido. Se entiende que
		se llama a este metodo cuando la entidad muere y vuelve a ser
		despertada.
		*/
		virtual void onActivate();

		void setOwner(Logic::CEntity *owner);

	protected:

		virtual void onStart();

		virtual void onFixedTick(unsigned int msecs);

		//Funci�n de cuando se tocan el lifeDome
		void lifeDomeTouched(CEntity *entityTouched);

		CGraphics* _cGraph;
		/*
		Puntero al componente fisico de la bala, se tiene por optimizacion
		*/
		CPhysicDynamicEntity *_physicComponent;

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================

		CEntity*		_owner;		

		float			_capsuleRadius;


		float			_scale;

		unsigned int _life;
		unsigned int _lifePerFriend;

	};//class CLifeDome

	REG_FACTORY(CLifeDome);

} // namespace Logic

#endif // __Logic_LifeDome_H