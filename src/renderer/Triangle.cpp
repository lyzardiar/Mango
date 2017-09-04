#include "Triangle.h"

namespace RE {
	Triangle Triangle::Default = {
		{
			{ 0, 0,		1, 1, 1, 1,		0, 0 },
			{ 100, 0,	1, 1, 1, 1,		1, 0 },
			{ 0, 100,	1, 1, 1, 1,		0, 1 },
			{ 100, 100,	1, 1, 1, 1,		1, 1 },
		},
		{ 0, 1, 2, 1, 2, 3 }
	};
}

