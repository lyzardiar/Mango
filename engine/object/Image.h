#pragma once
#include "engine/object/GameObject.h"
#include "engine/component/TriangleRenderer.h"

namespace RE {
	class Image : public GameObject {
	public:
		Image();
		Image(const char* name);
		virtual const char* ClassName() override { return "Image"; }

	public:
		virtual void OnDraw(const Affine& viewMat) override;

		virtual void OnGUI() override;

	protected:
		bool init();

	public:
		TriangleRenderer* renderer = nullptr;
		Color color = Color::White;
	};
}