#include "ImageInfo.h"
#include <thread>

using namespace RE;

static bool isInitAlphaMap = false;
static unsigned char alphaMap[256][256];

void ImageInfo::premultipliedAlpha() {
	assert(format == ImageInfo::PixelFormat::RGBA8888, "The pixel format should be RGBA8888!");

#define PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)((unsigned)(alphaMap[vr][va]) | \
				((unsigned)(alphaMap[vg][va]) << 8) | \
				((unsigned)(alphaMap[vb][va]) << 16) | \
				((unsigned)(va) << 24) \
			  )

	if (!isInitAlphaMap) {
		for (int i = 0; i < 256; ++i) {
			for (int j = 0; j < 256; ++j) {
				alphaMap[i][j] = (i * (j + 1)) >> 8;
			}
		}
		isInitAlphaMap = true;
	}

	auto job = [this](int startRow, int endRow) {
		unsigned int* fourBytes = (unsigned int*)data.getBytes() + startRow * width;
		int nLen = (endRow - startRow) * width;
		unsigned int* ptr = (unsigned int*)fourBytes;
		for (int i = 0; i < nLen; i++)
		{
			unsigned char* p = (unsigned char*)ptr++;
			fourBytes[i] = PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
		}
	};

	if (height > 512) {
		const int coreNum = 4;
		int seg[32];
		int segLen = height / coreNum;
		for (int i = 0; i < coreNum; ++i) {
			seg[i] = segLen * i;
		}
		seg[coreNum] = height;

		std::vector<std::thread> threads;
		for (int i = 0; i < coreNum; ++i) {
			threads.push_back(std::thread(std::bind(job, seg[i], seg[i + 1])));
		}
		for (int i = 0; i < coreNum; ++i) {
			threads[i].join();
		}
	}
	else {
		job(0, height);
	}

	isPreMultipliedAlpha = true;
}