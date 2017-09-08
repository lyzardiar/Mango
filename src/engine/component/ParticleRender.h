#pragma once
#include "core/base/Types.h"

namespace RE {
#define MAX_VERTICLES (1<<16)
	class ParticleRender {

		struct GPUParticleData {
		public:
			float* posx;
			float* posy;
			float* startPosX;
			float* startPosY;

			float* colorR;
			float* colorG;
			float* colorB;
			float* colorA;

			float* deltaColorR;
			float* deltaColorG;
			float* deltaColorB;
			float* deltaColorA;

			float* size;
			float* deltaSize;
			float* rotation;
			float* deltaRotation;
			float* timeToLive;
			unsigned int* atlasIndex;

			//! Mode A: gravity, direction, radial accel, tangential accel
			struct {
				float* dirX;
				float* dirY;
				float* radialAccel;
				float* tangentialAccel;
			} modeA;

			//! Mode B: radius mode
			struct {
				float* angle;
				float* degreesPerSecond;
				float* radius;
				float* deltaRadius;
			} modeB;

			unsigned int maxCount;
			GPUParticleData();
			bool init(int count);
			void release();
			unsigned int getMaxCount() { return maxCount; }

			void copyParticle(int p1, int p2) {
				posx[p1] = posx[p2];
				posy[p1] = posy[p2];
				startPosX[p1] = startPosX[p2];
				startPosY[p1] = startPosY[p2];

				colorR[p1] = colorR[p2];
				colorG[p1] = colorG[p2];
				colorB[p1] = colorB[p2];
				colorA[p1] = colorA[p2];

				deltaColorR[p1] = deltaColorR[p2];
				deltaColorG[p1] = deltaColorG[p2];
				deltaColorB[p1] = deltaColorB[p2];
				deltaColorA[p1] = deltaColorA[p2];

				size[p1] = size[p2];
				deltaSize[p1] = deltaSize[p2];

				rotation[p1] = rotation[p2];
				deltaRotation[p1] = deltaRotation[p2];

				timeToLive[p1] = timeToLive[p2];

				atlasIndex[p1] = atlasIndex[p2];

				modeA.dirX[p1] = modeA.dirX[p2];
				modeA.dirY[p1] = modeA.dirY[p2];
				modeA.radialAccel[p1] = modeA.radialAccel[p2];
				modeA.tangentialAccel[p1] = modeA.tangentialAccel[p2];

				modeB.angle[p1] = modeB.angle[p2];
				modeB.degreesPerSecond[p1] = modeB.degreesPerSecond[p2];
				modeB.radius[p1] = modeB.radius[p2];
				modeB.deltaRadius[p1] = modeB.deltaRadius[p2];

			}
		};

		struct ParticleVertex {
			Vec2 pos;
			Color color;
			float rotation;
			float size;
		};

	};
}
