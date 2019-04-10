#pragma once

namespace RE {
	class Renderer {
	public:
		static Renderer* CurrentRenderer;
	public:
		Renderer();


	public:
		class RenderState& rs;
	};
}