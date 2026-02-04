#ifndef XNA_FRAMEWORK_GAME_GRAPHICSDEVICEINFORMATION_HPP
#define XNA_FRAMEWORK_GAME_GRAPHICSDEVICEINFORMATION_HPP

#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Framework/Graphics/Shared.hpp"
#include "Xna/Framework/Graphics/PresentationParameters.hpp"

namespace Xna {
	//Holds the settings for creating a graphics device on Windows.
	class GraphicsDeviceInformation {
	public:
		//Initializes a new instance of this class.
		GraphicsDeviceInformation() = default;

		//Specifies which graphics adapter to create the device on.
		inline GraphicsAdapter Adapter() const { return *adapter; }
		//Specifies which graphics adapter to create the device on.
		inline void Adapter(GraphicsAdapter const& value) { adapter = value; }
		//Gets the graphics profile, which determines the graphics feature set.
		inline Xna::GraphicsProfile GraphicsProfile() const { return profile; }
		//Gets the graphics profile, which determines the graphics feature set.
		inline void GraphicsProfile(Xna::GraphicsProfile const& value) { profile = value; }
		//Specifies the presentation parameters to use when creating a graphics device.
		inline Xna::PresentationParameters& PresentationParameters() { return presentationParameters; } //TODO: precisa retorna uma referência?
		//Specifies the presentation parameters to use when creating a graphics device.
		inline void PresentationParameters(Xna::PresentationParameters const& value) { presentationParameters = value; }

		bool operator==(const GraphicsDeviceInformation& other) const {
			return adapter == other.adapter && profile == other.profile && presentationParameters == other.presentationParameters;
		}

	private:
		std::optional<GraphicsAdapter> adapter = GraphicsAdapter::DefaultAdapter();
		Xna::GraphicsProfile profile{};
		Xna::PresentationParameters presentationParameters{};	

		friend class GraphicsDeviceManager;
		friend struct GraphicsDeviceInformationComparer;
	};
}

#endif