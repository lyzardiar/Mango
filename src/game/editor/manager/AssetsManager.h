#pragma once
#include "core/base/Array.h"
#include "core/base/String.h"
#include "core/base/Path.h"
#include <functional>
#include <memory>
#include <mutex>
#include <vector>


namespace RE {
	class FileSystemWatcherTask {
	public:
		virtual int task() { return 0; }

		std::shared_ptr<bool> quit;
	};
	
	class FileSystemWatcher {
	public:
		FileSystemWatcher() : quit ( new bool(false)) {  }
		FileSystemWatcher(std::shared_ptr<bool> sq) : quit(sq){}
		~FileSystemWatcher() {
			if (task != nullptr) {
				delete task; 
				task = nullptr;
			}
		}
		bool start(const char* path);

		std::shared_ptr<bool> quit;
		FileSystemWatcherTask* task = nullptr;
		std::function<void(const char*, int)> callback = nullptr;
	};

	class AssetsManager {
	public:
		static AssetsManager instance;

	public:
		~AssetsManager();

		typedef Path PathType;
		enum class ResType {
			ALL,
			IMAGE,
			SHADER,
			SCRIPT
		};

		enum class FileChangeType {
			NONE,
			MODIFY,
			ADDED,
			REMOVED,
			RENAME_NEW,
			RENAME_OLD,
		};

		struct ChangedFile {
			ChangedFile(Path& f, FileChangeType t) : file(f), type(t) {}
			Path file;
			FileChangeType type;
		};

		class ThreadQueue : public std::list<ChangedFile> {
		public:
			void Push(ChangedFile& file) {
				std::lock_guard<std::recursive_mutex> ulock(_mutex);

				insert(end(), file);
			}

			ChangedFile& Get() {
				std::lock_guard<std::recursive_mutex> ulock(_mutex);

				return back();
			}

			bool Empty() {
				std::lock_guard<std::recursive_mutex> ulock(_mutex);

				return empty();
			}

			int Size() {
				std::lock_guard<std::recursive_mutex> ulock(_mutex);

				return size();
			}

			ChangedFile Pop() {
				std::lock_guard<std::recursive_mutex> ulock(_mutex);

				ChangedFile file = front();
				erase(begin());
				return file;
			}


		protected:
			std::recursive_mutex _mutex;
		};

	public:
		void ScanFold(const char* fold);

		void AddFile(PathType& file);
		void RemoveFile(PathType& file);

		void StartFileSystemWatcher(const char* path, std::function<void (const char*, int)> callback);
		void StopFileSystemWatcher();

		void Update(float dt);

		void FileChanged(const char* path, FileChangeType type);
	protected:
		bool addCatagrayFile(std::vector<PathType>& container, const PathType& file);
		bool removeCatagrayFile(std::vector<PathType>& container, const PathType& file);
	public:
		PathType curDir;
		std::vector<PathType> files;
		std::vector<PathType> imageFiles;
		std::vector<PathType> shaderFiles;
		std::vector<PathType> scriptFiles;

		FileSystemWatcher* fileSystemWatcher = nullptr;
		std::shared_ptr<bool> quitFileSystemWatcher;
		ThreadQueue changedFiles;
	};
}