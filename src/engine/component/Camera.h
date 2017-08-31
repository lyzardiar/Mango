#pragma once
#include "IComponent.h"

namespace RE {

	// just for projection matrix
	class Camera : public IComponent {
	public:
		static Camera& instance;

	public:
		ComponentTypeName(Camera)

		Camera() 
			: size(0, 0)
		{
			memset(matp, 0, sizeof(matp));
		}
		
	public:
		void Set2D(int width, int height) {
			if (size.width == width && size.height == height) {
				return;
			}

			size.width = width;
			size.height = height;

			width /= 2;
			height /= 2;
			
			float left = -(float)width;
			float right = (float)width;
			float bottom = -(float)height;
			float top = (float)height;

			float zNearPlane = -1000;
			float zFarPlane = 1000;

			memset(matp, 0, sizeof(float) * 16);
			matp[0] = 2 / (right - left);
			matp[5] = 2 / (top - bottom);
			matp[10] = 2 / (zNearPlane - zFarPlane);

			matp[12] = (left + right) / (left - right);
			matp[13] = (top + bottom) / (bottom - top);
			matp[14] = (zNearPlane + zFarPlane) / (zNearPlane - zFarPlane);
			matp[15] = 1;
		}

	public: 
		Vec2i size;

		float matp[16];
	};
}