/**
@file Conversions.h

Funciones auxiliares para convertir entre los tipos de PhysX y los tipos de la l�gica. 

@author Francisco Aisa Garc�a.
@date Marzo, 2013.
*/

#ifndef	__Physics_Conversions_H_
#define	__Physics_Conversions_H_

#include "BaseSubsystems/Math.h"

#include <foundation/PxVec3.h> 
#include <foundation/PxTransform.h>
#include <foundation/PxMat44.h>
#include <characterkinematic/PxExtended.h>

namespace Physics {

	/**
	Transforma un vector l�gico al equivalente en PhysX.

	@param v Vector l�gico a transformar.
	@return Vector de physX equivalente.
	*/
	inline physx::PxVec3 Vector3ToPxVec3(const Vector3 &v) {
		return physx::PxVec3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un vector de PhysX al equivalente l�gico.

	@param v Vector de physX que queremos transformar.
	@return El vector l�gico equivalente.
	*/
	inline Vector3 PxVec3ToVector3(const physx::PxVec3 &v) {
		return Vector3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un quaternion de Physx al equivalente l�gico.

	@param q Quaternion de physx que queremos transformar.
	@return Quaternion l�gico equivalente.
	*/
	inline Quaternion PxQuatToQuaternion(const physx::PxQuat &q) {
		return Quaternion(q.w, q.x, q.y, q.z);
	}

	//________________________________________________________________________

	/**
	Transforma un quaternion de la l�gica al equivalente en Physx.

	@param q Quaternion de la logica que queremos transformar.
	@return Quaternion f�sico equivalente.
	*/
	inline physx::PxQuat QuaternionToPxQuat(const Quaternion &q) {
		return physx::PxQuat(q.x, q.y, q.z, q.w);
	}

	//________________________________________________________________________


	/**
	Transforma un vector l�gico al equivalente vector extendido de PhysX.

	@param v Vector l�gico que queremos transformar.
	@return Vector de physX equivalente.
	*/
	inline physx::PxExtendedVec3 Vector3ToPxExtendedVec3(const Vector3 &v) {
		return physx::PxExtendedVec3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un vector extendido de PshysX al equivalente l�gico.

	@param v Vector de physX que queremos transformar.
	@return Vector l�gico equivalente.
	*/
	inline Vector3 PxExtendedVec3ToVector3(const physx::PxExtendedVec3 &v) {
		return Vector3((float) v.x, (float) v.y, (float) v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un Transform de PhysX a una matriz 4x4 l�gica equivalente.

	@param t Transform de physX.
	@return Matriz de rotaci�n equivalente a la de physX.
	*/
	inline Matrix4 PxTransformToMatrix4(const physx::PxTransform &t) {
		physx::PxMat44 m(t);

		return Matrix4(m(0,0), m(0,1), m(0,2), m(0,3), 
					   m(1,0), m(1,1), m(1,2), m(1,3),
					   m(2,0), m(2,1), m(2,2), m(2,3),
					   0, 0, 0, 1);
	}

	//________________________________________________________________________

	/**
	Transforma una matriz 4x4 l�gica en un Transform de PhysX equivalente.

	@param m Matriz l�gica que queremos transformar.
	@return Transform de physX equivalente a la matriz de rotaci�n dada.
	*/
	inline physx::PxTransform Matrix4ToPxTransform(const Matrix4 &m) {
		physx::PxMat44 pm(physx::PxVec4(m[0][0], m[1][0], m[2][0], m[3][0]),
						  physx::PxVec4(m[0][1], m[1][1], m[2][1], m[3][1]),
						  physx::PxVec4(m[0][2], m[1][2], m[2][2], m[3][2]),
						  physx::PxVec4(m[0][3], m[1][3], m[2][3], m[3][3]));

		return physx::PxTransform(pm);
	}

}; // namespace Physics

#endif // __Physics_Conversions_H_