#ifndef XNA_FRAMEWORK_CONTENT__CONTENTMANAGER_HPP
#define XNA_FRAMEWORK_CONTENT__CONTENTMANAGER_HPP

#include <memory>
#include "Xna/CSharp/IO/FileStream.hpp"
#include "Xna/Framework/Game/GameServiceContainer.hpp"

namespace Xna {
	class ContentReader;

	//The ContentManager is the run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager {
	public:
		//Initializes a new instance of ContentManager.
		ContentManager(GameServiceContainer const& serviceProvider);
		//Initializes a new instance of ContentManager.
		ContentManager(GameServiceContainer const& serviceProvider, std::filesystem::path const& rootDirectory);

		GameServiceContainer ServiceProvider() const;

		//Gets or sets the root directory associated with this ContentManager.
		std::filesystem::path RootDirectory() const;
		//Gets or sets the root directory associated with this ContentManager.
		void RootDirectory(std::filesystem::path const& value);

		//Disposes all data that was loaded by this ContentManager.
		virtual void Unload() {}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T> T Load(std::filesystem::path const& assetName);

		inline ContentManager(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ContentManager const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		template <typename T> T ReadAsset(std::filesystem::path const& assetName);
		std::unique_ptr<CSharp::Stream> OpenStream(std::filesystem::path const& assetName);

	public:
		struct Implementation;
		std::shared_ptr<Implementation> impl;	
	};
}

#endif