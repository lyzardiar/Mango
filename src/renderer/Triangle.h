#pragma once

#include "core/base/Types.h"
#include "core/base/Array.h"
#include <vector>

namespace RE {
	struct Triangle {
		struct Vertex {
			union { float position[2]; struct { float x, y; }; };
			union { float color[4]; struct { float r, g, b, a; }; };
			union { float tex[2]; struct { float u, v; }; };
		};

		Array<Vertex> verts = Array<Vertex>(4);
		Array<UI16> indices = Array<UI16>(6);

		static Triangle Default;
	};

	struct Quad {
		struct Vertex {
			union { float position[2]; struct { float x, y; }; };
			union { float color[4]; struct { float r, g, b, a; }; };
			union { float tex[2]; struct { float u, v; }; };
		};

		Array<Vertex> verts = Array<Vertex>(4);
		Array<UI16> indices = Array<UI16>(4);
	};
}
