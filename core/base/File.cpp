#include "File.h"
#include <string>

namespace RE
{
	FileStream::FileStream()
	{

	}

	FileStream::FileStream(const char* path, const char* mode)
	{
		open(path, mode);
	}

	FileStream::~FileStream()
	{
		close();
	}

	bool FileStream::open(const char* path, const char* mode)
	{
		std::string filepath = path;
		_file.Init(filepath);
		return true;
	}

	void FileStream::close()
	{
		_file.Close();
	}

	bool FileStream::writeText(const char* text)
	{
		_file.AddString(text);
		return true;
	}

	bool FileStream::write(const void* data, size_t size)
	{
		_file.AddBuff(data, size);
		return true;
	}

	bool FileStream::read(void* data, size_t size)
	{
		if (data != nullptr) {
			auto buff = _file.Read(size);
			if (buff) {
				memcpy(data, buff, size);
			}
		}
		return true;
	}

	size_t FileStream::size()
	{
		return _file.Size();
	}

	size_t FileStream::pos()
	{
		return _file.SeekPos();
	}

	bool FileStream::seek(size_t pos, File::SeekMode mode)
	{
		_file.Seek(pos, mode);
		return true;
	}


	FileStream& FileStream::operator <<(const char* text)
	{
		writeText(text);
		return *this;
	}


	FileStream& FileStream::operator <<(I32 value)
	{
		char buff[20];
		sprintf(buff, "%d", value);
		writeText(buff);
		return *this;
	}


	FileStream& FileStream::operator <<(UI32 value)
	{
		char buff[20];
		sprintf(buff, "%u", value);
		writeText(buff);
		return *this;
	}


	FileStream& FileStream::operator <<(UI64 value)
	{
		char buff[40];
		sprintf(buff, "%llu", value);
		writeText(buff);
		return *this;
	}


	FileStream& FileStream::operator <<(float value)
	{
		char buff[30];
		sprintf(buff, "%f", value);
		writeText(buff);
		return *this;
	}

} // RE