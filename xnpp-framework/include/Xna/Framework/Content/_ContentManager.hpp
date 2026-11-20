#pragma once

#include <memory>
#include <Xna/CSharp/IO/FileStream.hpp>
#include "Xna/Framework/Game/GameServiceContainer.hpp"
#include "Xna/Macros/Api.hpp"

namespace Xna {
	class ContentReader;

	//The ContentManager is the run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager {
	public:		
		//Initializes a new instance of ContentManager.
		XNPP_API ContentManager(GameServiceContainer const& serviceProvider, std::filesystem::path const& rootDirectory = "");

		XNPP_API GameServiceContainer ServiceProvider() const;

		//Gets or sets the root directory associated with this ContentManager.
		XNPP_API std::filesystem::path RootDirectory() const;
		//Gets or sets the root directory associated with this ContentManager.
		XNPP_API void RootDirectory(std::filesystem::path const& value);

		//Disposes all data that was loaded by this ContentManager.
		virtual void Unload() {}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T> T Load(std::filesystem::path const& assetName);		

	protected:
		template <typename T> T ReadAsset(std::filesystem::path const& assetName);
		XNPP_API std::unique_ptr<CSharp::Stream> OpenStream(std::filesystem::path const& assetName);

	public:
		struct Implementation;
		std::unique_ptr<Implementation> impl;	
	};
}