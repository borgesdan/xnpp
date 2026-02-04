#ifndef XNA_FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_HPP
#define XNA_FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_HPP

#include "_GraphicsDevice.hpp"
#include "Xna/Platform/Platform.hpp"
#include "GraphicsAdapter.hpp"
#include "Viewport.hpp"
#include "BlendState.hpp"
#include "DepthStencilState.hpp"
#include "RasterizerState.hpp"
#include "SamplerState.hpp"
#include "RenderTarget2D.hpp"
#include "RenderTargetBinding.hpp"
#include "VertexBufferBinding.hpp"
#include "TextureCollection.hpp"

#include <optional>

namespace Xna {
	struct GraphicsDevice::Implementation {
		CSharp::Event<CSharp::EventArgs> deviceResetting;
		CSharp::Event<CSharp::EventArgs> deviceReset;
		CSharp::Event<CSharp::EventArgs> deviceLost;
		CSharp::Event<CSharp::EventArgs> disposing;

		Xna::PresentationParameters presentationParameters{};
		Xna::GraphicsProfile graphicsProfile{};
		std::optional<GraphicsAdapter> adapter;
		std::vector<Xna::Viewport> viewports{1};
		std::vector<Rectangle> scissorRectangles{ 1 };

		std::vector<RenderTarget2D> current2DRenderTargets{ 1 };		

		std::optional<Xna::BlendState> blendState;
		std::optional<Xna::DepthStencilState> depthStencilState;
		std::optional<Xna::RasterizerState> rasterizerState;

		SamplerStateCollection samplerState;
		SamplerStateCollection vertexSamplerState;

		int32_t multiSampleMask{ -1 };
		int32_t referenceStencil{ 0 };

		bool lazyInitialization{ false };

		PlatformImpl::GraphicsDeviceImpl platformImpl;
	};	

	inline GraphicsAdapter GraphicsDevice::Adapter() const { return implGraphicsDevice->adapter.value(); }
	inline Xna::BlendState GraphicsDevice::BlendState() const { return implGraphicsDevice->blendState.value(); }
	inline Color GraphicsDevice::BlendFactor() const { return implGraphicsDevice->blendState->impl->blendFactor; }
	inline Xna::DepthStencilState GraphicsDevice::DepthStencilState() const { return implGraphicsDevice->depthStencilState.value(); }
	inline int32_t GraphicsDevice::MultiSampleMask() const { return implGraphicsDevice->multiSampleMask; }
	inline Xna::PresentationParameters& GraphicsDevice::PresentationParameters() { return implGraphicsDevice->presentationParameters; }
	inline Xna::RasterizerState GraphicsDevice::RasterizerState() const { return implGraphicsDevice->rasterizerState.value(); }
	inline int32_t GraphicsDevice::ReferenceStencil() const { return implGraphicsDevice->referenceStencil; }
	inline SamplerStateCollection GraphicsDevice::SamplerStates() const { return implGraphicsDevice->samplerState; }
	inline Xna::Viewport GraphicsDevice::Viewport() const { return implGraphicsDevice->viewports[0]; }
	inline Rectangle GraphicsDevice::ScissorRectangle() const { return implGraphicsDevice->scissorRectangles[0]; }
	inline RenderTarget2D GraphicsDevice::CreateBackBufferRenderTarget() const { return Platform::GraphicsDevice_CreateBackBufferRenderTarget(*this); };
	inline void GraphicsDevice::Clear(Color const& color) { Clear(ClearOptions::Target, color, 0, 0); }
	inline void GraphicsDevice::Reset() { Reset(implGraphicsDevice->presentationParameters); }
	inline void GraphicsDevice::Reset(Xna::PresentationParameters const& presentationParameters) { Reset(presentationParameters, *implGraphicsDevice->adapter); }
	inline void GraphicsDevice::Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) { 
		Platform::GraphicsDevice_Reset(*this, presentationParameters, graphicsAdapter); 
	}

	inline std::vector<RenderTargetBinding> GraphicsDevice::GetRenderTargets() { return std::vector<RenderTargetBinding>(); } // TODO
	inline std::vector<VertexBufferBinding> GraphicsDevice::GetVertexBuffers() { return std::vector<VertexBufferBinding>(); } // TODO
	inline TextureCollection GraphicsDevice::Textures() const { return TextureCollection(); } //TODO

	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceResetting() { return implGraphicsDevice->deviceResetting; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceReset() { return implGraphicsDevice->deviceReset; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceLost() { return implGraphicsDevice->deviceLost; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::Disposing() { return implGraphicsDevice->disposing; }
}

#endif