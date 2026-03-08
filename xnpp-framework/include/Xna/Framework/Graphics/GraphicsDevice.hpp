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

		Xna::BlendState blendState{};
		Xna::DepthStencilState depthStencilState{};
		Xna::RasterizerState rasterizerState{};

		SamplerStateCollection samplerState;
		SamplerStateCollection vertexSamplerState;

		int32_t multiSampleMask{ -1 };
		int32_t referenceStencil{ 0 };

		bool lazyInitialization{ false };
		
		std::unique_ptr<PlatformNS::IGraphicsDevice> backend;
	};	

	inline GraphicsAdapter GraphicsDevice::Adapter() const { return impl->adapter.value(); }
	inline Xna::BlendState GraphicsDevice::BlendState() const { return impl->blendState; }
	inline Color GraphicsDevice::BlendFactor() const { return impl->blendState.BlendFactor; }
	inline Xna::DepthStencilState GraphicsDevice::DepthStencilState() const { return impl->depthStencilState; }
	inline int32_t GraphicsDevice::MultiSampleMask() const { return impl->multiSampleMask; }
	inline Xna::PresentationParameters& GraphicsDevice::PresentationParameters() { return impl->presentationParameters; }
	inline Xna::RasterizerState GraphicsDevice::RasterizerState() const { return impl->rasterizerState; }
	inline int32_t GraphicsDevice::ReferenceStencil() const { return impl->referenceStencil; }
	inline SamplerStateCollection GraphicsDevice::SamplerStates() const { return impl->samplerState; }
	inline Xna::Viewport GraphicsDevice::Viewport() const { return impl->viewports[0]; }
	inline Rectangle GraphicsDevice::ScissorRectangle() const { return impl->scissorRectangles[0]; }
	
	inline RenderTarget2D GraphicsDevice::CreateBackBufferRenderTarget() const { 
		//TODO: Implementar CreateBackBufferRenderTarget
		return { nullptr };
	};

	inline void GraphicsDevice::Clear(Color const& color) { Clear(ClearOptions::Target, color, 0, 0); }
	inline void GraphicsDevice::Reset() { Reset(impl->presentationParameters); }
	inline void GraphicsDevice::Reset(Xna::PresentationParameters const& presentationParameters) { Reset(presentationParameters, *impl->adapter); }
	inline void GraphicsDevice::Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) { 
		impl->backend->Reset(presentationParameters, graphicsAdapter);
	}

	inline std::vector<RenderTargetBinding> GraphicsDevice::GetRenderTargets() { return std::vector<RenderTargetBinding>(); } // TODO
	inline std::vector<VertexBufferBinding> GraphicsDevice::GetVertexBuffers() { return std::vector<VertexBufferBinding>(); } // TODO
	inline TextureCollection GraphicsDevice::Textures() const { return TextureCollection(); } //TODO

	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceResetting() { return impl->deviceResetting; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceReset() { return impl->deviceReset; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::DeviceLost() { return impl->deviceLost; }
	inline CSharp::Event<CSharp::EventArgs> GraphicsDevice::Disposing() { return impl->disposing; }

	inline PlatformNS::IGraphicsDevice& GraphicsDevice::GetBackend() {
		return *impl->backend.get();
	}
}

#endif