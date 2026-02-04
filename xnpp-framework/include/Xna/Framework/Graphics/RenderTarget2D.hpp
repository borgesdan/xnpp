#ifndef XNA_FRAMEWORK_GRAPHICS_RENDERTARGET2D_HPP
#define XNA_FRAMEWORK_GRAPHICS_RENDERTARGET2D_HPP

#include "Texture2D.hpp"
#include "Xna/Platform/Platform.hpp"
#include "IDynamicGraphicsResource.hpp"

namespace Xna {
	//Contains a 2D texture that can be used as a render target. 
	class RenderTarget2D final : public IDynamicGraphicsResource {
	public:
		//Initializes a new instance of this class.
		RenderTarget2D(
			GraphicsDevice& graphicsDevice,
			size_t width,
			size_t height
		) : RenderTarget2D(graphicsDevice, width, height, false,
			SurfaceFormat::Color, DepthFormat::None, 0, Xna::RenderTargetUsage::DiscardContents) {}

		//Initializes a new instance of this class.
		RenderTarget2D(
			GraphicsDevice& graphicsDevice,
			size_t width,
			size_t height,
			bool mipMap,
			SurfaceFormat preferredFormat,
			DepthFormat preferredDepthFormat
		) : RenderTarget2D(graphicsDevice, width, height, mipMap,
			preferredFormat, preferredDepthFormat, 0, Xna::RenderTargetUsage::DiscardContents) {}

		//Initializes a new instance of this class.
		RenderTarget2D(
			GraphicsDevice& graphicsDevice,
			size_t width,
			size_t height,
			bool mipMap,
			SurfaceFormat preferredFormat,
			DepthFormat preferredDepthFormat,
			size_t preferredMultiSampleCount,
			Xna::RenderTargetUsage usage
		);

		~RenderTarget2D() override = default;

		//Gets the data format for the depth stencil data.
		inline DepthFormat DepthStencilFormat() const { return impl->depthStencilFormat; }
		//Gets the number of sample locations during multisampling.
		inline size_t MultiSampleCount() const { return impl->multiSampleCount; }
		inline Xna::RenderTargetUsage RenderTargetUsage() { return impl->usage; }		

		inline RenderTarget2D(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(RenderTarget2D const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

		inline Xna::Texture2D Texture2D() const { return impl->texture2D; }

		//TODO: [!] Remover isso depois
		inline RenderTarget2D() { 
			impl = std::make_shared<Implementation>(); 
			impl->texture2D = Xna::Texture2D();
		}

	private:
		struct Implementation {
			Xna::Texture2D texture2D{ nullptr };

			DepthFormat depthStencilFormat{};
			size_t multiSampleCount{ 0 };
			Xna::RenderTargetUsage usage{};
			CSharp::Event<CSharp::EventArgs> contentLost;
			PlatformImpl::RenderTarget2DImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;	
	
	public:
		//
		//IDynamicGraphicsResource
		//

		virtual CSharp::Event<CSharp::EventArgs> ContentLost() const override {
			return impl->contentLost;
		}
		
		inline virtual bool IsContentLost() const override {
			return false;
		}
		
		inline virtual void SetContentLost(bool IsContentLost) {
			
		}

		friend struct Platform;
	};
}

#endif