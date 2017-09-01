#pragma once

#ifdef _WIN32 
#pragma warning(disable: 4996)

#include <stdlib.h>  
#include <string.h>  
#include <stdio.h>   
#include <iostream>
#include <string>
#include <vector>
#include <direct.h>
#include <io.h>

#include "core/base/String.h"
#include "core/base/Types.h"

#include<Shlwapi.h> 
#pragma comment(lib,"Shlwapi.lib")

#define ACCESS _access
#define MKDIR(a) _mkdir((a))

using namespace std;

namespace RE {
	class FileDirWin {
	public:
		static bool CreatDir(std::string dirPath) {
			int i = 0;
			int iRet;
			int iLen = dirPath.size();
			if (dirPath[iLen - 1] != '\\' && dirPath[iLen - 1] != '/') {
				dirPath.append("\\");
			}

			// create dir
			for (i = 0; i < iLen; i++) {
				if (dirPath[i] == '\\' || dirPath[i] == '/') {
					dirPath[i] = '\0';

					iRet = ACCESS(dirPath.c_str(), 0);
					if (iRet != 0) {
						iRet = MKDIR(dirPath.c_str());
						if (iRet != 0) {
							return false;
						}
					}
					dirPath[i] = '\\';
				}
			}
			return true;
		}

		static bool SHDeleteFolder(const char* pstrFolder) {
			int iPathLen = strlen(pstrFolder);
			TCHAR tczFolder[MAX_PATH + 1];
			SHFILEOPSTRUCT FileOp;

			if ((nullptr == pstrFolder)) {
				return false;
			}


			if (iPathLen >= MAX_PATH) {
				return true;
			}

			/*确保目录的路径以2个\0结尾*/
			ZeroMemory(tczFolder, (MAX_PATH + 1) * sizeof(TCHAR));
			strcpy(tczFolder, pstrFolder);
			tczFolder[iPathLen] = '\0';
			tczFolder[iPathLen + 1] = '\0';

			ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
			FileOp.fFlags |= FOF_SILENT;            //不显示进度
			FileOp.fFlags |= FOF_NOERRORUI;         //不报告错误信息
			FileOp.fFlags |= FOF_NOCONFIRMATION;    //直接删除，不进行确认
			FileOp.hNameMappings = nullptr;
			FileOp.hwnd = nullptr;
			FileOp.lpszProgressTitle = nullptr;
			FileOp.wFunc = FO_DELETE;
			FileOp.pFrom = tczFolder;               //要删除的目录，必须以2个\0结尾
			FileOp.pTo = nullptr;

			FileOp.fFlags &= ~FOF_ALLOWUNDO;       //直接删除，不放入回收站

			if (0 == SHFileOperation(&FileOp)) {
				return true;
			}
			else {
				puts(RE::String::Format("Del folder: [%s] Failed.", pstrFolder).c_str());
				return false;
			}
		}

		static std::string Normalized(std::string path) {
			path = RE::String::Replace(path, "/", "\\");
			if (path[path.size() - 1] != '\\' && path[path.size() - 1] != '/') path.append("\\");

			std::vector<std::string> paths;
			std::string subpath;
			int pos = 0;
			int size = path.size();
			while (pos < size) {
				if (path[pos] == '\\' || path[pos] == '/') {
					if (subpath == "..") {
						// pop fore path
						paths.resize(paths.size() - 1);
					}
					else if (subpath == ".") {
						// do nothing
					}
					else {
						if (subpath != "")
							paths.push_back(subpath);
					}
					subpath.clear();
				}
				else {
					subpath += path[pos];
				}
				++pos;
			}
			subpath.clear();
			for (auto& sub : paths) {
				subpath += (sub + '\\');
			}
			return subpath;
		}

		static void Del(std::string& path) {
			SHDeleteFolder(path.c_str());
		}
	public:
		FileDirWin();
		FileDirWin(string dir) {
			init(dir);
		}

		const char* GetDirName() {
			return _dir;
		}

		bool init(string dir) {
			if (_fullpath(_dir, dir.c_str(), _MAX_PATH) == NULL)
				return false;

			bool ret = false;

			char buffer[MAX_PATH];
			_getcwd(buffer, MAX_PATH);
			if (_chdir(_dir) == 0) {
				int len = strlen(_dir);
				if (_dir[len - 1] != '\\')
					strcat(_dir, "\\");

				ret = true;
			}
			_chdir(buffer);
			return ret;
		}


		bool BeginBrowseDirOnce() {
			Clear();
			ProcessDir(_dir, nullptr);
			return BrowseDirOnce(_dir);
		}
		bool BeginBrowseFileOnce(const char *filespec) {
			Clear();
			ProcessDir(_dir, nullptr);
			return BrowseFileOnce(_dir, filespec);
		}

		bool BeginBrowse(const char *filespec) {
			Clear();
			ProcessDir(_dir, nullptr);
			return BrowseDir(_dir, filespec);
		}

		bool BrowseDir(const char *dir, const char *filespec) {
			long hFile;
			_finddata_t fileinfo;

			char buffer[MAX_PATH];
			_getcwd(buffer, MAX_PATH);

			// files
			_chdir(dir);
			if ((hFile = _findfirst(filespec, &fileinfo)) != -1) {
				do {
					if (!(fileinfo.attrib & _A_SUBDIR)) {
						char filename[_MAX_PATH] = { 0 };
						strcpy(filename, dir);
						strcat(filename, fileinfo.name);
						if (!ProcessFile(filename)) {
							_chdir(buffer);
							return false;
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);
				_findclose(hFile);
			}

			// dirs
			_chdir(dir);
			if ((hFile = _findfirst("*.*", &fileinfo)) != -1) {
				do {
					if ((fileinfo.attrib & _A_SUBDIR)) {
						if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
							char subdir[_MAX_PATH];
							strcpy(subdir, dir);
							strcat(subdir, fileinfo.name);
							strcat(subdir, "\\");
							ProcessDir(subdir, dir);
							if (!BrowseDir(subdir, filespec)) {
								_chdir(buffer);
								return false;
							}
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);
				_findclose(hFile);
			}

			_chdir(buffer);
			return true;
		}

		bool BrowseFileOnce(const char *dir, const char *filespec) {
			long hFile;
			_finddata_t fileinfo;

			char buffer[MAX_PATH];
			_getcwd(buffer, MAX_PATH);

			_chdir(dir);
			if ((hFile = _findfirst(filespec, &fileinfo)) != -1) {
				do {
					if (!(fileinfo.attrib & _A_SUBDIR)) {
						char filename[_MAX_PATH] = { 0 };
						strcpy(filename, dir);
						strcat(filename, fileinfo.name);
						if (!ProcessFile(filename)) {
							_chdir(buffer);
							return false;
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);
				_findclose(hFile);
			}

			_chdir(buffer);
		}

		bool BrowseDirOnce(const char *dir) {
			long hFile;
			_finddata_t fileinfo;

			char buffer[MAX_PATH];
			_getcwd(buffer, MAX_PATH);

			_chdir(dir);
			if ((hFile = _findfirst("*.*", &fileinfo)) != -1) {
				do {
					if ((fileinfo.attrib & _A_SUBDIR)) {
						if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
							char subdir[_MAX_PATH];
							strcpy(subdir, dir);
							strcat(subdir, fileinfo.name);
							strcat(subdir, "\\");
							ProcessDir(subdir, dir);
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);
				_findclose(hFile);
			}
			_chdir(buffer);
			return true;
		}

		bool ProcessFile(const char *filename) {
			_files.push_back(filename);
			return true;
		}

		void ProcessDir(const char *currentdir, const char *parentdir) {
			_dirs.push_back(currentdir);
		}

		void Clear() {
			_files.clear();
			_dirs.clear();
		}

		vector<string>& Files() { return _files; }
		vector<string>& Dirs() { return _dirs; }

	protected:

		vector<string> _files;
		vector<string> _dirs;

		char _dir[1024];
	};
}

#endif