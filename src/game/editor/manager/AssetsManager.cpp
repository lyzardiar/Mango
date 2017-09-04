#include "AssetsManager.h"

#include "platform/win32/FileHelperWin32.h"
#include <assert.h>
#include <thread>
#include "ScriptManager.h"

RE::AssetsManager RE::AssetsManager::instance;


RE::AssetsManager::~AssetsManager() {
	if (fileSystemWatcher != nullptr) {
		delete fileSystemWatcher;
		fileSystemWatcher = nullptr;
	}
}

void RE::AssetsManager::ScanFold(const char* fold) {
	FileDirWin fdir(".");
	curDir = fdir.GetDirName();

	fdir.BeginBrowse("*.*");

	auto& vfiles = fdir.Files();

	PathType spath;
	for (auto& path : vfiles) {
		path = String::Replace(path, curDir.data, ""); 		
		spath = String::Replace(path, "\\", "/");
		AddFile(spath);
	}
	
}

void RE::AssetsManager::AddFile(PathType& file) {
	if (file.EndWith(".png") || file.EndWith(".jpg")) {
		addCatagrayFile(imageFiles, file);
	}
	else if (file.EndWith(".lua")) {
		addCatagrayFile(scriptFiles, file);
	}
	else if (file.EndWith(".frag") || file.EndWith(".vert")) {
		addCatagrayFile(shaderFiles, file);
	}
	addCatagrayFile(files, file);
}

void RE::AssetsManager::RemoveFile(PathType& file) {
	if (file.EndWith(".png") || file.EndWith(".jpg")) {
		removeCatagrayFile(imageFiles, file);
	}
	else if (file.EndWith(".lua")) {
		removeCatagrayFile(scriptFiles, file);
	}
	else if (file.EndWith(".frag") || file.EndWith(".vert")) {
		removeCatagrayFile(shaderFiles, file);
	}
	removeCatagrayFile(files, file);
}

void RE::AssetsManager::StartFileSystemWatcher(const char* path, std::function<void(const char*, int)> callback) {
	quitFileSystemWatcher = make_shared<bool>(false);
	fileSystemWatcher = new FileSystemWatcher(quitFileSystemWatcher);

	fileSystemWatcher->callback = callback;
	fileSystemWatcher->start(path);
}

void RE::AssetsManager::StopFileSystemWatcher() {
	*quitFileSystemWatcher = true;
}

void RE::AssetsManager::Update(float dt) {
	if (changedFiles.Empty()) return;

	static StaticString<128> lastFile;

	auto item = changedFiles.Pop();
	lastFile = item.file;
	if (RE::AssetsManager::FileChangeType::RENAME_OLD == item.type) {
		return;
	}

	bool isScript = item.file.EndWith(".lua");

	switch (item.type)
	{
	case RE::AssetsManager::FileChangeType::NONE:
		break;
	case RE::AssetsManager::FileChangeType::MODIFY:
		if (isScript) ScriptManager::instance.UpdateScript(item.file);
		break;
	case RE::AssetsManager::FileChangeType::ADDED:
		AddFile(item.file);
		break;
	case RE::AssetsManager::FileChangeType::REMOVED:
		RemoveFile(item.file);
		if (isScript) ScriptManager::instance.RemoveScript(item.file);
		break;
	case RE::AssetsManager::FileChangeType::RENAME_NEW:
		RemoveFile(lastFile);
		AddFile(item.file);
		if (isScript) ScriptManager::instance.RenameScript(lastFile, item.file);
		break;
	default:
		break;
	}
}

void RE::AssetsManager::FileChanged(const char* path, FileChangeType type) {
	std::string sp(path);
	sp = String::Replace(sp, "\\", "/");
	changedFiles.Push(ChangedFile(PathType(sp), type));
}

bool RE::AssetsManager::addCatagrayFile(Array<PathType>& container, const PathType& file) {
	bool ret = false;
	if (container.Find(file) == -1) {
		container.Push(file);
		ret = true;
	}
	return ret;
}

bool RE::AssetsManager::removeCatagrayFile(Array<PathType>& container, const PathType& file) {
	bool ret = false;
	if (container.Find(file) != -1) {
		container.Remove(file);
		ret = true;
	}
	return ret;
}

namespace RE {
	class FileSystemWatcherTaskWin;

	static const DWORD READ_DIR_CHANGE_FILTER =
		FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_ACCESS |
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME;

	static void wcharToCharArray(const WCHAR* src, char* dest, int len);
	static void CALLBACK notif(DWORD status, DWORD tferred, LPOVERLAPPED over);

	class FileSystemWatcherTaskWin : public FileSystemWatcherTask {
	public:
		FileSystemWatcherTaskWin(const char* path, FileSystemWatcher& watcher)
			: _path(path)
			, _watcher(watcher)
		{

		}

		int task() override {
			_handle = CreateFile(_path.data,
				FILE_LIST_DIRECTORY,
				FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
				nullptr,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
				nullptr);
			if (_handle == INVALID_HANDLE_VALUE) return -1;

			memset(&_overlapped, 0, sizeof(_overlapped));
			_overlapped.hEvent = this;
			*quit = false;
			while (! (*quit)) {
				//puts("Change handling");
				BOOL status = ReadDirectoryChangesW(_handle,
					_info,
					sizeof(_info),
					TRUE,
					READ_DIR_CHANGE_FILTER,
					&_received,
					&_overlapped,
					&notif);
				if (status == FALSE) break;
				SleepEx(INFINITE, TRUE);
			}
			return 0;
		}

		FILE_NOTIFY_INFORMATION _info[10];
		HANDLE _handle;
		DWORD _received;
		OVERLAPPED _overlapped;

		StaticString<128> _path;
		FileSystemWatcher& _watcher;
	};

	static void wcharToCharArray(const WCHAR* src, char* dest, int len) {
		for (unsigned int i = 0; i < len / sizeof(WCHAR); ++i) {
			dest[i] = static_cast<char>(src[i]);
		}
		dest[len / sizeof(WCHAR)] = '\0';
	}

	static void CALLBACK notif(DWORD status, DWORD tferred, LPOVERLAPPED over) {
		auto* task = (FileSystemWatcherTaskWin*)over->hEvent;
		if (status == ERROR_OPERATION_ABORTED) {
			*task->quit = true;
			return;
		}

		FILE_NOTIFY_INFORMATION* info = &task->_info[0];
		while (info) {
			auto action = info->Action;
			char tmp[MAX_PATH];
			wcharToCharArray(info->FileName, tmp, info->FileNameLength);

			switch (action) {
			case FILE_ACTION_RENAMED_NEW_NAME:
			{
				puts(StaticString<256>("ReName New: ", tmp).data);
				if (task->_watcher.callback != nullptr) task->_watcher.callback(tmp, (int)AssetsManager::FileChangeType::RENAME_NEW);
				break;
			}
			case FILE_ACTION_ADDED:
			{
				puts(StaticString<256>("Added: ", tmp).data);
				if (task->_watcher.callback != nullptr) task->_watcher.callback(tmp, (int)AssetsManager::FileChangeType::ADDED);
				break;
			}
			case FILE_ACTION_MODIFIED:
			{
				puts(StaticString<256>("Modify: ", tmp).data);
				if (task->_watcher.callback != nullptr) task->_watcher.callback(tmp, (int)AssetsManager::FileChangeType::MODIFY);
				break;
			}
			case FILE_ACTION_REMOVED:
			{
				puts(StaticString<256>("Deleted: ", tmp).data);
				if (task->_watcher.callback != nullptr) task->_watcher.callback(tmp, (int)AssetsManager::FileChangeType::REMOVED);
				break;
			}
			case FILE_ACTION_RENAMED_OLD_NAME:
			{
				puts(StaticString<256>("ReName Old: ", tmp).data);
				if (task->_watcher.callback != nullptr) task->_watcher.callback(tmp, (int)AssetsManager::FileChangeType::RENAME_OLD);
				break;
			}
			default: assert(false); break;
			}
			info = info->NextEntryOffset == 0
				? nullptr
				: (FILE_NOTIFY_INFORMATION*)(((char*)info) + info->NextEntryOffset);
		}
	}
}


bool RE::FileSystemWatcher::start(const char* path) {
	task = new FileSystemWatcherTaskWin(path, *this);
	task->quit = quit;
	std::thread trd([this]() {
		task->task();
	});

	trd.detach();

	return true;
}
