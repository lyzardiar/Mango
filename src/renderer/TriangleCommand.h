#pragma once
#include "IRenderCommand.h"

namespace RE {
	class TriangleCommand : public IRenderCommand {
	public:
		virtual void Commit() override;
	};
}