#pragma once
#include "core/base/Array.h"
#include "engine/component/ScriptComponent.h"

namespace RE {

	class ScriptManager {
	public:
		static ScriptManager instance;

	public:
		void Add(ScriptComponent* comp);
		void Remove(ScriptComponent* comp);

		void AddScript(Path& path);
		void RemoveScript(Path& path);
		void UpdateScript(Path& path);
		void RenameScript(Path& lastpath, Path& curpath);
	public:
				
		Array<ScriptComponent*> scriptList;
	};
}
