#include "Data.h"

using namespace RE;

const Data Data::Null;

Data::Data() :
	_bytes(nullptr),
	_size(0)
{

}

Data::Data(Data&& other) :
	_bytes(nullptr),
	_size(0)
{
	move(other);
}

Data::Data(const Data& other) :
	_bytes(nullptr),
	_size(0)
{
	copy(other._bytes, other._size);
}

Data::~Data()
{
	clear();
}

Data& Data::operator= (const Data& other)
{
	copy(other._bytes, other._size);
	return *this;
}

Data& Data::operator= (Data&& other)
{
	move(other);
	return *this;
}

void Data::move(Data& other)
{
	_bytes = other._bytes;
	_size = other._size;

	other._bytes = nullptr;
	other._size = 0;
}

bool Data::isNull() const
{
	return (_bytes == nullptr || _size == 0);
}

unsigned char* Data::getBytes() const
{
	return _bytes;
}

size_t Data::getSize() const
{
	return _size;
}

void Data::copy(const unsigned char* bytes, const size_t size)
{
	clear();

	if (size > 0)
	{
		_size = size;
		_bytes = (unsigned char*)malloc(_size + 1);
		_bytes[_size] = 0;
		memcpy(_bytes, bytes, _size);
	}
}

void Data::fastSet(unsigned char* bytes, const size_t size)
{
	_bytes = bytes;
	_size = size;
}

void Data::clear()
{
	if (_bytes != nullptr)
		free(_bytes);
	_bytes = nullptr;
	_size = 0;
}