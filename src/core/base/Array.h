#pragma once
#include <initializer_list>

namespace RE {
	template<typename T>
	class Array {
	public:
		Array() {
			capacity = 1;
			data = (T*)malloc(sizeof(T) * capacity);
		}
		Array(int size) 
			: capacity(size)
		{
			data = (T*)malloc(sizeof(T) * capacity);
		}

		Array(const std::initializer_list<T> &v) {
			Resize((int)v.size());
			int idx = 0;
			for (const T& item : v) {
				data[idx++] = item;
			}
			size = idx;
		}

		Array(const Array& other) {
			Copy(other);
		}

		Array(const Array&& other) {
			Move(other);
		}

		const Array& operator = (const Array& other) {
			Copy(other);

			return *this;
		}

		const Array& operator = (const Array&& other) {
			Move(other);

			return *this;
		}
		
		T& operator [] (int idx) {
			return data[idx];
		}

		~Array() {
			free(data);
			data = nullptr;
		}

		T& Get(int idx) {
			return data[idx];
		}

		int Find(const T& obj) {
			for (int idx = 0; idx < size; ++idx) {
				if (data[idx] == obj) return idx;
			}
			return -1;
		}

		int Find(const T& obj) const {
			for (int idx = 0; idx < size; ++idx) {
				if (data[idx] == obj) return idx;
			}
			return -1;
		}

		const T& Push(const T& val) {
			if (size >= capacity) {
				Resize(capacity * 2);
			}
			data[size++] = val;
			return data[size-1];
		}

		void Push(const Array& vals) {
			while (size + (vals.size-1) >= capacity) {
				Resize(capacity * 2);
			}
			for (int i = 0; i < vals.size; ++i) {
				data[size++] = vals[i];
			}
		}

		void Remove(int idx) {
			if (idx < 0 || idx >= size) return;
			--size;
			memmove(data + idx, data + idx + 1, sizeof(T) * (size - idx));
		}

		void Remove(const T& val) {
			auto idx = Find(val);
			if (idx != -1) {
				Remove(idx);
			}
		}

		void Resize(int cap) {
			if (cap <= capacity) return;
			capacity = cap;
			
			data = (T*)realloc(data, sizeof(T) * capacity);
		}

		void Copy(const Array& other) {
			if (data != nullptr) {
				free(data);
			}

			data = (T*)malloc(sizeof(T) * other.capacity);
			memcpy(data, other.data, sizeof(T) * other.capacity);
			capacity = other.capacity;
			size = other.size;
		}

		void Move(const Array& other) {
			data = other.data;
			capacity = other.capacity;
			size = other.size;

			other.data = nullptr;
			other.capacity = 0;
			other.size = 0;
		}

		void Clear() {
			size = 0;
		}

	public:
		T* data = nullptr;
		int size = 0;
		int capacity = 0;
	};


	template<typename T, int N>
	class StaticArray {
	public:
		StaticArray() {  }
		StaticArray(const std::initializer_list<T> &v) {
			int idx = 0;
			for (const T& item : v) {
				data[idx++] = item;
			}
			size = idx;
		}

		const T& Push(const T& val) {
			assert(size < capacity);
			data[size++] = val;
			return val;
		}

		void Remove(int idx) {
			if (idx < 0 || idx >= size) return;
			--size;
			memmove(data + idx, data + idx + 1, sizeof(T) * (size - idx));
		}

		T& Get(int idx) {
			return data[idx];
		}

	public:
		T data[N];
		int size = 0;
		const int capacity = N;
	};
}