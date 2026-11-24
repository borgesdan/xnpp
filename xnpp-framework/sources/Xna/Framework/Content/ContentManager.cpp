#include "Xna/Framework/Content/ContentManager.hpp"
#include "Xna/Framework/TitleContainer.hpp"
#include "../TitleLocation.hpp"
#include <Xna/CSharp/Path.hpp>

namespace Xna {
	ContentManager::ContentManager(GameServiceContainer* serviceProvider, std::filesystem::path const& rootDirectory) {		
		RootDirectory(rootDirectory);
		serviceProvider = serviceProvider;	
	}

	GameServiceContainer* ContentManager::ServiceProvider() const {
		return serviceProvider;
	}

	std::filesystem::path ContentManager::RootDirectory() const {
		return rootDirectory;
	}

	void ContentManager::RootDirectory(std::filesystem::path const& value) {
		rootDirectory = value;
		fullRootDirectory = value;
		isRootDirectoryAbsolute = TitleContainer::IsPathAbsolute(value);

		if (!isRootDirectoryAbsolute)
			return;

		fullRootDirectory = CSharp::Path::Combine(TitleLocation::Path(), value);
	}

	std::unique_ptr<CSharp::Stream> ContentManager::OpenStream(std::filesystem::path const& assetName) {
		auto asset = assetName;
		asset.replace_extension(".xnb");
		auto path = CSharp::Path::Combine(fullRootDirectory, asset);

		auto cleanPath = TitleContainer::GetCleanPath(path);

		if (isRootDirectoryAbsolute)
		{
			auto stream = new CSharp::FileStream(cleanPath, CSharp::FileMode::Open, CSharp::FileAccess::Read);
			return std::unique_ptr<CSharp::FileStream>(stream);
		}

		return TitleContainer::OpenStream(cleanPath);
	}
}