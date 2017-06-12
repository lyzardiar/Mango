#include "ImageDecoder.h"
#include "core/platform/FileUtils.h"
#include "PngDecoder.h"

using namespace RE;

bool isPng(const unsigned char * data, I32 dataLen) {
	if (dataLen <= 8) {
		return false;
	}
	static const unsigned char PNG_SIGNATURE[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
	return memcmp(PNG_SIGNATURE, data, sizeof(PNG_SIGNATURE)) == 0;
}

bool isJpg(const unsigned char * data, I32 dataLen) {
	if (dataLen <= 4) {
		return false;
	}
	static const unsigned char JPG_SOI[] = { 0xFF, 0xD8 };
	return memcmp(data, JPG_SOI, 2) == 0;
}

ImageInfo ImageDecoder::decode(std::string& path) {
	ImageInfo info = Png::Decode(path);

	return info;
}
