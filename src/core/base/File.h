#pragma once

#include <sys/stat.h>
#include <string>
#include <vector>

#include "Types.h"
#include "String.h"
#include "Path.h"

namespace RE {
#if defined(_MSC_VER)
#define STAT_ struct _stat64
#define SATA_G _stat64
#else 
#define STAT_ struct stat
#define SATA_G stat
#endif

#if !defined(S_ISREG)
#  define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

	class FileState {
	public:
		FileState(const std::string& filepath = "") {
			init(filepath);
		}
		FileState(Path& filepath) {
			init(filepath.data);
		}

		bool init(const std::string& filepath = "") {
			if (filepath == "") return false;

			int ret = SATA_G(filepath.c_str(), &_stats);
			if (ret != 0) return false;
			if (S_ISREG(_stats.st_mode) == 0) {
				size = 0;
			}
			else {
				size = (int)_stats.st_size;
			}

			isDir = (_stats.st_mode & S_IFDIR) != 0;

			return true;
		}

	public:
		int size = 0;
		bool isDir = false;

		STAT_ _stats;
	};

	class File {
	public:
		static void Copy(Path from, Path to) {
			File f(from), t(to);
			t.Write(f.Buff(), f.Size());
		}

		static bool Del(std::string& path) {
			return remove(path.c_str()) == 0;
		}
	public:
		enum class SeekMode {
			BEGIN = 0,
			END,
			CURRENT,
		};
	public:
		File() {
		}

		File(Path $filepath) {
			Init($filepath.data);
		}

		void Init(const std::string& $filepath = "") {
			Close();

			filePath = $filepath;

			int pos = filePath.rfind("/");
			if (pos != std::string::npos) {
				dirPath = filePath.substr(0, pos);
				baseName = filePath.substr(pos + 1);
			}
			else {
				baseName = filePath;
			}

			size = FileState(filePath).size;
		}

		File& CopyTo(std::string& to) {
			File::Copy(filePath, to);
			return *this;
		}

		File& Remove() {
			Close();
			File::Del(filePath);
			return *this;
		}

		void Write(const void* writeBuff, int buffSize) {
			do {
				Close();

				FILE *fp = fopen(filePath.c_str(), "wb+");
				if (!fp) break;
				if (writeBuff != nullptr && buffSize > 0) {
					size = fwrite(writeBuff, sizeof(unsigned char), buffSize, fp);
				}
				fclose(fp);
			} while (0);
		}

		template<class T>
		File& AddNumber(T number) {
			return AddBuff(&number, sizeof(T));
		}

		File& AddString(const char* str) {
			return AddBuff(str, strlen(str));
		}

		File& AddBuff(const void* writeBuff, int buffSize) {
			do
			{
				if (filePtr == nullptr) {
					filePtr = fopen(filePath.c_str(), "wb+");
					if (filePtr) fclose(filePtr);

					filePtr = fopen(filePath.c_str(), "ab+");
				}
				if (filePtr == nullptr) {
					printf("File can not open: %s", filePath.c_str());
					return *this;
				}

				fwrite(writeBuff, sizeof(unsigned char), buffSize, filePtr);

			} while (0);
			return *this;
		}

		void Seek(I32 pos, SeekMode mode = SeekMode::BEGIN) {
			if (filePtr == nullptr) {
				return;
			}
			switch (mode)
			{
			case SeekMode::BEGIN:
				fseek(filePtr, pos, SEEK_SET);
				break;
			case SeekMode::END:
				fseek(filePtr, pos, SEEK_END);
				break;
			case SeekMode::CURRENT:
				fseek(filePtr, pos, SEEK_CUR);
				break;
			default:
				break;
			}
		}

		UI32 SeekPos() {
			if (filePtr == nullptr) {
				return 0;
			}
			return offset > 0 ? offset : ftell(filePtr);
		}

		UI8* Read(int ts) {
			readBuff();
			if (offset + ts > size) return nullptr;

			UI8* ret = buff + offset;
			offset += ts;
			return ret;
		}

		UI8* ReadLine(int& s) {
			readBuff();
			if (offset >= size) return nullptr;

			UI8* ret = buff + offset;
			UI8* ptr = ret;

			int os = 0;
			while (*ptr) {
				if (*ptr == '\n') {
					os++;
					break;
				}
				os++;
				ptr++;
			}
			offset += os;

			while (*ptr == '\n' || *ptr == '\r') {
				*ptr = '\0';
				ptr--;
				os--;
			}

			s = os;
			return ret;
		}

		UI8* ReadHead(int ts) {
			static UI8 header[128];
			do {
				Close();

				FILE *fp = fopen(filePath.c_str(), "rb");
				if (!fp) break;

				ts = fread(header, sizeof(UI8), ts, fp);
				header[ts] = 0;
				fclose(fp);
			} while (0);

			return header;
		}

		template<class T>
		T Read(bool endian = true) {
			readBuff();
			int ts = sizeof(T);

			if (offset + ts > size) return 0;

			T ret;
			memcpy(&ret, buff + offset, ts);
			offset += ts;

			if (endian) swapEndian((UI8*)&ret, ts);
			return ret;
		}

		void Reset() {
			offset = 0;
		}

		File& DropBuff() {
			buff = nullptr;
			Close();
			return *this;
		}

		unsigned char* Buff() {
			return readBuff();
		}

		int Size(bool init = false) {
			if (init) readBuff();
			return size;
		}

		bool Exists() {
			return size != 0;
		}

		File& Close() {
			if (buff != nullptr) {
				delete[] buff;
				buff = nullptr;
			}

			if (filePtr != nullptr) {
				fclose(filePtr);
			}
			filePtr = nullptr;

			size = 0;
			isRead = false;
			offset = 0;

			return *this;
		}

		~File() {
			Close();
		}

	protected:
		void swapEndian(UI8* sb, int size) {
			int hs = size / 2;
			int idx = 0;
			while (idx <= hs - 1) {
				std::swap(*(sb + idx), *(sb + size - idx - 1));
				++idx;
			}
		}

		UI8* readBuff() {
			if (isRead) return buff;
			isRead = true;

			do {
				Close();

				size = FileState(filePath).size;

				FILE *fp = fopen(filePath.c_str(), "rb");
				if (!fp) break;

				buff = new UI8[size + 1];

				size = fread(buff, sizeof(UI8), size, fp);
				buff[size] = 0;
				fclose(fp);
			} while (0);

			return buff;
		}

	public:

		std::string dirPath;
		std::string filePath;
		std::string baseName;

	protected:
		bool isRead = false;
		unsigned char* buff = nullptr;

		FILE* filePtr = nullptr;

		int size = 0;
		int offset = 0;

	};

	class FileStream
	{
	public:
		FileStream();
		FileStream(const char* path, const char* mod);
		~FileStream();

		bool open(const char* path, const char* mode);
		void close();

		bool write(const void* data, size_t size);
		bool writeText(const char* text);
		bool read(void* data, size_t size);

		size_t size();
		size_t pos();

		bool seek(size_t pos, File::SeekMode mode);

		FileStream& operator <<(const char* text);
		FileStream& operator <<(char c) { write(&c, sizeof(c)); return *this; }
		FileStream& operator <<(I32 value);
		FileStream& operator <<(UI32 value);
		FileStream& operator <<(UI64 value);
		FileStream& operator <<(float value);

	protected:
		File _file;
	};
}