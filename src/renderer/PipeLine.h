#pragma once
#include "core/base/Array.h"
#include "Triangle.h"
#include "Material.h"

namespace RE {
	class PipeLine {
	public:
		static PipeLine instance;
	public:
		void AddCommand(class IRenderCommand* cmd);
		void AddCommandImt(class IRenderCommand* cmd);

		void Commit();
	public:
		Triangle triangles;
		Material commitMaterial;
	};
}