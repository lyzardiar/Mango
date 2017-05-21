#pragma once

#include "Types.h"

namespace RE {
	class Data
	{
	public:
		static const Data Null;
		Data();
		Data(const Data& other);
		Data(Data&& other);
		~Data();
		Data& operator= (const Data& other);
		Data& operator= (Data&& other);
		unsigned char* getBytes() const;
		UI32 getSize() const;
		void copy(const unsigned char* bytes, const UI32 size);
		void fastSet(unsigned char* bytes, const UI32 size);
		void clear();
		bool isNull() const;

	private:
		void move(Data& other);

	private:
		unsigned char* _bytes;
		UI32 _size;
	};
}
