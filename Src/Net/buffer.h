//---------------------------------------------------------------------------
// buffer.h
//---------------------------------------------------------------------------

/**
 * @file buffer.h
 *
 * Implementa un buffer din�mico que los objetos utilizan para serializarse o deserializarse
 *
 *
 * @author Juan A. Recio-Garc�a
 * @author Francisco Aisa Garc�a
 * @date Enero, 2013
 */

#ifndef __BUFFER_H
#define __BUFFER_H

#include "BaseSubsystems\Math.h"


namespace Net {

typedef unsigned char byte;

/**
 * Implementa un buffer din�mico que los objetos utilizan para serializarse o deserializarse
 */
class CBuffer{
public:

	/**
	 * Crea un buffer din�mico.
	 * @param initsize Tama�o inicial del buffer
	 * @param delta Indica cuanto crecer� el buffer cada vez que se necesite ampliarlo
     */
	CBuffer(size_t initsize=500, size_t delta=100);

	/**
	 * Constructor de copias. Usa Copy-on-write.
	 */
	CBuffer(const CBuffer& source);

	/**
	 * Constructor por par�metros. Construye un mensaje a partir de una ristra de bytes.
	 * Tras la construccion NO SE RECOLOCA EL PUNTERO DE LECTURA/ESCRITURA, hay que hacerlo
	 * manualmente.
	 * El propio buffer se encargar� de elminar los datos reservados en la cadena de bytes pasada.
	 * OJO!! se asume que el tama�o pasado es el tama�o l�gico y f�sico! por lo que la ristra de bytes
	 * que se pasa se espera que sea del tama�o exacto de los datos que contiene.
	 */
	CBuffer::CBuffer(byte* adoptBuffer, size_t bufferSize);

	/*
	 * Copy-on-write
	 */
	CBuffer& operator=(const CBuffer& source);

	/**
	 * Destructora
	 */
	~CBuffer();

	/**
	 * Devuelve el buffer como una secuencia de bytes
	 */
	byte* getbuffer();

	/**
	 * Devuelve el tama�o del buffer. 
	 * Se utilizar� normalmente para saber cuantos bytes devuelve getbuffer()
	 */
	size_t getSize();

	/**
	 * Reinicia el buffer.
	 * No modifica su tama�o actual
	 */
	void reset();

	/**
	 * Escribe datos en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void write(void* data, size_t datalength);

	/**
	 * Lee datos del buffer.
	 * Al hacer esto el buffer se "vacia"
	 * \param data es un puntero indicando a donde se deben copiar los datos desde el buffer
	 * \param datalength es el n�mero de datos (bytes) a leer
	 */
	void read (void* data, size_t datalength);

	/**
	 * Escribe un vector3 en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(const Vector3& data);

	void deserialize(Vector3& data);

	/**
	 * Escribe un float en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(float data);

	void deserialize(float& data);

	/**
	 * Escribe un entero en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(int data);

	void deserialize(int& data);
	/**
	 * Escribe un entero sin signo en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(unsigned int data);

	void deserialize(unsigned int& data);
	/**
	 * Escribe un string en el buffer, comprimiendolo a CRC32
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(const std::string& data, bool crc);

	void deserialize(std::string& data, int size);
	/**
	 * Escribe un unsigned char en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(unsigned char data);

	void deserialize(unsigned char& data);
	/**
	 * Escribe un boleano en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(bool data);

	void deserialize(bool& data);
	/**
	 * Escribe un caracter en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(char data);

	void deserialize(char& data);
	/**
	 * Escribe un caracter en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tama�o de los datos a escribir (n�mero de bytes)
	 */
	void serialize(const Matrix4& data);

	void deserialize(Matrix4& data);

protected:
	void realloc();

	// Wrapper para facilitar el uso de copy-on-write
	struct BufferWrapper {
		// Informacion del buffer
		byte* _begin;
		byte* _current;
		size_t _maxsize;
		size_t _size;
		size_t _delta;
		
		// Informacion del copy-on-write
		unsigned int _refCount;
	};

	BufferWrapper* _wrapperPtr;

private:
	void createOwnInstance();

};

} // namespace Net

#endif // _CBUFFER_H