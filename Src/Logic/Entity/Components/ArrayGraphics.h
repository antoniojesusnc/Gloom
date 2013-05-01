/**
@file Graphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica del jugador, es decir, todas las armas que este portara.

@see Logic::CGraphics
@see Logic::IComponent

@author Antonio Narv�ez
@date Enero, 2013
*/
#ifndef __Logic_ArrayGraphics_H
#define __Logic_ArrayGraphics_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CEntity;
	class CScene;
}

//declaraci�n de la clase
namespace Logic
{
/**
	Componente que se encarga de la representaci�n gr�fica de una entidad.
	En concreto se encarga de las entidades con representaciones gr�ficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan �ste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de cambio de posici�n y orientaci�n (matriz de 
	transformaci�n) mediante el mensaje SET_TRANSFORM.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	class CArrayGraphics : public IComponent
	{
		DEC_FACTORY(CArrayGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CArrayGraphics() : IComponent(), _actualWeapon(0), _graphicsEntities(), _numWeapons(0) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CArrayGraphics();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deber� cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_TRANSFORM.

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
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void onDeactivate();

		/**
		Metodo para cambiar el grafico en funcion del arma actual
		*/
		void changeWeapon(int newWeapon);


		/**
		Aqui se actualizara la funcion, la saco fuera para hacer uso de ella
		*/
		void setTransform(const Matrix4 &transform);

	protected:

		/**
		M�todo virtual que construye la entidad gr�fica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gr�ficas (animadas, etc.).
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(std::string nombreEntidad, std::string modelo);

		/**
		metodo donde se hara el cambio de arma.
		*/
		


		


		/**
		arma actual equipada
		*/
		int _actualWeapon;
		
		/**
		Estructura donde se guardara el offset y las modificaciones en el arma
		*/
		struct TGraphicsWeapon{
			Graphics::CEntity *_graphicsEntity;
			Vector3 *offset;
			float yaw;
			float pitch;
		};

		/**
		Entidades gr�fica.
		*/
		TGraphicsWeapon *_graphicsEntities;

		/**
		Aqui almaceno el numero de aramas (lo mas seguro que esto deba de ser borrado) !!!! cuidado que en el destructor se usa
		*/
		int _numWeapons;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		//Graphics::SceneNode* _scene;
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CArrayGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
