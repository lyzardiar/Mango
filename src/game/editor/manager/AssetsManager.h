#pragma once
#include "core/base/Array.h"
#include "core/base/String.h"


namespace RE {
	class AssetsManager {
	public:
		static AssetsManager instance;

	public:
		typedef StaticString<128> PathType;
		enum class ResType {
			ALL,
			IMAGE,
			SHADER,
			SCRIPT
		};

	public:
		void ScanFold(const char* fold);

		void AddFile(PathType& file);

	protected:
		bool addCatagrayFile(Array<PathType>& container, const PathType& file);
	public:
		PathType curDir;
		Array<PathType> files;
		Array<PathType> imageFiles;
		Array<PathType> shaderFiles;
		Array<PathType> scriptFiles;
	};
}