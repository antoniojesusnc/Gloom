//---------------------------------------------------------------------------
// StaticEntity.h
//---------------------------------------------------------------------------

/**
@file StaticEntity.h

Contiene la declaraci�n de la clase que representa a las entidades f�sicas
est�ticas.

@see Physics::CStaticEntity

@author Francisco Aisa Garc�a
@date Marzo, 2013
*/

#ifndef __Physics_StaticEntity_H
#define __Physics_StaticEntity_H

#include "BaseSubsystems/Math.h"
#include "Physics/Entity.h"
#include "GeometryFactory.h"
#include "MaterialManager.h"

#include <vector>
#include <string>

namespace Physics {

	/**
	Clase que representa a las entidades f�sicas est�ticas.

	IMPORTANTE: Actualmente solo permitimos la carga de actores que
	se componen de un solo shape. En el futuro convendr�a cambiar esto
	ya que un mismo actor puede estar compuesto de varios shapes distintos.

	@ingroup physicsGroup

	@author Francisco Aisa Garc�a
	@date Marzo, 2013
	*/

	class CStaticEntity : public CEntity {
	public:


		// =======================================================================
		//                     METODOS HEREDADOS DE CEntity
		// =======================================================================


		/**
		Carga una entidad f�sica desde un fichero RepX.

		@param file Fichero desde el que se van a leer los datos.
		@param group Grupo de colisi�n que queremos asignar al actor.
		@param groupList Grupos de colisi�n con los que el actor quiere interactuar.
		@param component Componente l�gico asociado.
		*/
		virtual void load(const std::string &file, int group, const std::vector<int>& groupList, const Logic::IPhysics *component);
		

		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/** 
		Crea un plano est�tico a partir de los par�metros dados.

		@param plane Geometr�a del plano que queremos crear.
		@param material Material que queremos que tenga el plano.
		@param group Grupo de colisi�n del plano.
		@param groupList Grupos de colisi�n con los que queremos que interactue el plano.
		@param component Componente l�gico asociado.
		*/
		void load(const PlaneGeometry& plane, Material& material, int group,
				  const std::vector<int>& groupList, const Logic::IPhysics* component);

		//________________________________________________________________________

		/** 
		Crea una entidad est�tica a partir de los par�metros dados.

		@param position Posicion en la que queremos crear la entidad. Notar que una vez
		seteada, ya no podemos moverla de posici�n.
		@param geometry Geometr�a de la entidad est�tica. Por ser una entidad est�tica
		soporta cualquier geometr�a.
		@param material Material del que queremos que est� formada la entidad.
		@param trigger True si es un trigger.
		@param group Grupo de colisi�n de la entidad.
		@param groupList Grupos de colisi�n con los que queremos que la entidad interactue.
		@param component Componente l�gico asociado.
		*/
		void load(const Matrix4& transform, const Geometry& geometry, Material& material, 
				  bool trigger, int group, const std::vector<int>& groupList, const Logic::IPhysics* component);

	}; // class CEntity

} // namespace Physics

#endif // __Physics_Entity_H
