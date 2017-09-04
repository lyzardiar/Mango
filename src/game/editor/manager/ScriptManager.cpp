#include "ScriptManager.h"



RE::ScriptManager RE::ScriptManager::instance;

void RE::ScriptManager::Add(ScriptComponent* comp) {
	scriptList.Push(comp);
}

void RE::ScriptManager::Remove(ScriptComponent* comp) {
	scriptList.Remove(comp);
}

void RE::ScriptManager::AddScript(StaticString<128>& path) {
	int size = scriptList.size;
	for (int idx = 0; idx < size; ++idx) {
		auto comp = scriptList[idx];
		if (path == comp->path) {
			comp->isValid = true;
		}
	}
}

void RE::ScriptManager::RemoveScript(StaticString<128>& path) {
	int size = scriptList.size;
	for (int idx = 0; idx < size; ++idx) {
		auto comp = scriptList[idx];
		if (path == comp->path) {
			comp->isValid = false;
		}
	}
}

void RE::ScriptManager::UpdateScript(StaticString<128>& path) {
	int size = scriptList.size;
	for (int idx = 0; idx < size; ++idx) {
		auto comp = scriptList[idx];
		if (path == comp->path) {
			comp->Reload();
		}
	}
}

void RE::ScriptManager::RenameScript(StaticString<128>& lastpath, StaticString<128>& curpath) {
	int size = scriptList.size;
	for (int idx = 0; idx < size; ++idx) {
		auto comp = scriptList[idx];
		if (lastpath == comp->path) {
			comp->path = curpath.data;
		}
	}
}
