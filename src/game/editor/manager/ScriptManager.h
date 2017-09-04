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

		void AddScript(StaticString<128>& path);
		void RemoveScript(StaticString<128>& path);
		void UpdateScript(StaticString<128>& path);
		void RenameScript(StaticString<128>& lastpath, StaticString<128>& curpath);
	public:
				
		Array<ScriptComponent*> scriptList;
	};
}
