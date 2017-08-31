#pragma once

#include "core/base/Types.h"
#include "core/base/Array.h"
#include <vector>

namespace RE {
	struct Triangle {
		struct Vertex {
			union { float position[3]; struct { float x, y, z; }; };
			union { float color[4]; struct { float r, g, b, a; }; };
			union { float tex[2]; struct { float u, v; }; };
		};

		Array<Vertex> verts = Array<Vertex>(4);
		Array<UI16> indices = Array<UI16>(4);

		static Triangle Default;
	};

	struct Quad {
		struct Vertex {
			float position[3];
			float color[4];
			float tex[2];
		};

		Array<Vertex> verts = Array<Vertex>(4);
		Array<UI16> indices = Array<UI16>(4);
	};
}
