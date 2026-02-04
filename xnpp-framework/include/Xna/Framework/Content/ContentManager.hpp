#ifndef XNA_FRAMEWORK_CONTENT_CONTENTMANAGER_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTMANAGER_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include "Xna/CSharp/Exception.hpp"
#include "ContentReader.hpp"
#include "_ContentManager.hpp"

namespace Xna {
	struct ContentManager::Implementation {		
		std::filesystem::path rootDirectory;
		std::filesystem::path fullRootDirectory;
		bool isRootDirectoryAbsolute{ false };
		GameServiceContainer serviceProvider;
		std::shared_ptr<std::vector<uint8_t>> byteBuffer;
	};	

	template <typename T> T ContentManager::Load(std::filesystem::path const& assetName) {
		if (assetName.empty())
			throw CSharp::InvalidOperationException("assetName is empty.");

		auto obj3 = ReadAsset<T>(assetName);

		return obj3;
	}
	template <typename T> T ContentManager::ReadAsset(std::filesystem::path const& assetName) {
		std::shared_ptr<CSharp::Stream> input = OpenStream(assetName);

		ContentReader contentReader = ContentReader::Create(*this, input, assetName);
		return contentReader.ReadAsset<T>();
	}
}

#endif