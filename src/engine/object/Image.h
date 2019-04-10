#pragma once
#include "engine/object/GameObject.h"
#include "engine/component/TriangleRenderer.h"

namespace RE {
	class Image : public GameObject {
	public:
		Image(const char* name = "");
		virtual const char* ClassName() override { return "Image"; }

	public:
		virtual void OnDraw(const Affine& viewMat) override;

		virtual void OnGUI() override;

		void setTexture(const char* path);
	protected:
		virtual  bool init() override;

	public:
		TriangleRenderer* renderer = nullptr;
	};
}