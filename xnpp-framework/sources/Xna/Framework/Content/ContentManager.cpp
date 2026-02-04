#include "Xna/Framework/Content/ContentManager.hpp"
#include "Xna/Framework/TitleContainer.hpp"
#include "Xna/Framework/TitleLocation.hpp"
#include "Xna/CSharp/Path.hpp"

namespace Xna {
	ContentManager::ContentManager(GameServiceContainer const& serviceProvider) {
		impl = std::make_shared<ContentManager::Implementation>();
		impl->serviceProvider = serviceProvider;
	}

	ContentManager::ContentManager(GameServiceContainer const& serviceProvider, std::filesystem::path const& rootDirectory) {
		impl = std::make_shared<ContentManager::Implementation>();
		RootDirectory(rootDirectory);
		impl->serviceProvider = serviceProvider;	
	}

	GameServiceContainer ContentManager::ServiceProvider() const {
		return impl->serviceProvider;
	}

	std::filesystem::path ContentManager::RootDirectory() const {
		return impl->rootDirectory;
	}

	void ContentManager::RootDirectory(std::filesystem::path const& value) {
		impl->rootDirectory = value;
		impl->fullRootDirectory = value;
		impl->isRootDirectoryAbsolute = TitleContainer::IsPathAbsolute(value);

		if (!impl->isRootDirectoryAbsolute)
			return;

		impl->fullRootDirectory = CSharp::Path::Combine(TitleLocation::Path(), value);
	}

	std::unique_ptr<CSharp::Stream> ContentManager::OpenStream(std::filesystem::path const& assetName) {
		auto asset = assetName;
		asset.replace_extension(".xnb");
		auto path = CSharp::Path::Combine(impl->fullRootDirectory, asset);

		auto cleanPath = TitleContainer::GetCleanPath(path);

		if (impl->isRootDirectoryAbsolute)
		{
			auto stream = new CSharp::FileStream(cleanPath, CSharp::FileMode::Open, CSharp::FileAccess::Read);
			return std::unique_ptr<CSharp::FileStream>(stream);
		}

		return TitleContainer::OpenStream(cleanPath);
	}
}