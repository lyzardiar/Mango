#include "PngDecoder.h"

#include "png.h"
#include "core/platform/FileUtils.h"

using namespace RE;

typedef struct
{
	const unsigned char * data;
	I32 size;
	int offset;
}tImageSource;

static inline void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

	if ((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}


ImageInfo Png::Decode(std::string& path)
{
	Data data = FileUtils::getInstance()->getData(path);
	return Decode(data.getBytes(), data.getSize());
}

ImageInfo Png::Decode(UI8* data, UI32 dataLen)
{
#define PNGSIGSIZE  8
#define BREAK_IF(cond) if (cond) break
	bool ret = false;
	png_byte        header[PNGSIGSIZE] = { 0 };
	png_structp     png_ptr = 0;
	png_infop       info_ptr = 0;

	ImageInfo imageInfo;

	do
	{
		// png header len is 8 bytes
		BREAK_IF(dataLen < PNGSIGSIZE);

		// check the data is png or not
		memcpy(header, data, PNGSIGSIZE);
		BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

		// init png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		BREAK_IF(!png_ptr);

		// init png_info
		info_ptr = png_create_info_struct(png_ptr);
		BREAK_IF(!info_ptr);

		BREAK_IF(setjmp(png_jmpbuf(png_ptr)));

		// set the read call back function
		tImageSource imageSource;
		imageSource.data = (unsigned char*)data;
		imageSource.size = dataLen;
		imageSource.offset = 0;
		png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

		// read png header info
		// read png file info
		png_read_info(png_ptr, info_ptr);

		imageInfo.width = png_get_image_width(png_ptr, info_ptr);
		imageInfo.height = png_get_image_height(png_ptr, info_ptr);
		png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

		//CCLOG("color type %u", color_type);

		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			bit_depth = 8;
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		// reduce images with 16-bit samples to 8 bits
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
		}

		// Expanded earlier for grayscale, now take care of palette and rgb
		if (bit_depth < 8)
		{
			png_set_packing(png_ptr);
		}
		// update info
		png_read_update_info(png_ptr, info_ptr);
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		switch (color_type)
		{
		case PNG_COLOR_TYPE_GRAY:
			imageInfo.format = ImageInfo::PixelFormat::I8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			imageInfo.format = ImageInfo::PixelFormat::AI88;
			break;
		case PNG_COLOR_TYPE_RGB:
			imageInfo.format = ImageInfo::PixelFormat::RGB888;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			imageInfo.format = ImageInfo::PixelFormat::RGBA8888;
			break;
		default:
			break;
		}

		// read png data
		png_size_t rowbytes;
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * imageInfo.height);


		rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		auto imgSize = rowbytes * imageInfo.height;
		auto imgData = static_cast<unsigned char*>(malloc(imgSize * sizeof(unsigned char)));

		if (!imgSize)
		{
			if (row_pointers != nullptr)
			{
				free(row_pointers);
			}
			break;
		}
		imageInfo.data.fastSet(imgData, imgSize);

		for (unsigned short i = 0; i < imageInfo.height; ++i)
		{
			row_pointers[i] = imgData + i*rowbytes;
		}
		png_read_image(png_ptr, row_pointers);

		png_read_end(png_ptr, nullptr);

		// premultiplied alpha for RGBA8888
		if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			imageInfo.premultipliedAlpha();
		}
		else
		{
			imageInfo.isPreMultipliedAlpha = false;
		}

		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}

		ret = true;
	} while (0);

	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}
	imageInfo.valid = ret;
	return imageInfo;
}

RE::ImageInfo RE::Png::Decode(const char* path)
{
	std::string pathname = path;
	return Decode(pathname);
}
