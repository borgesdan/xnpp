#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
    GraphicsDevice::GraphicsDevice(GraphicsAdapter const& adapter, Xna::GraphicsProfile const& graphicsProfile, Xna::PresentationParameters const& presentationParameters) {
        assert(adapter != nullptr && "GraphicsAdapter is null.");

        impl = std::make_shared<Implementation>();
        impl->backend = PlatformNS::IGraphicsDevice::Create();

        impl->adapter = adapter;
        impl->graphicsProfile = graphicsProfile;
        impl->presentationParameters = presentationParameters;       

        impl->backend->CreateDevice(adapter, presentationParameters);
    }    

    void GraphicsDevice::MultiSampleMask(int32_t value) {
        impl->multiSampleMask = value;
        //TODO: MultiSampleMask não é aplicado no backend
    }    

    void GraphicsDevice::ReferenceStencil(int32_t value) {
        impl->referenceStencil = value;
        //TODO: ReferenceStencil não é aplicado no backend
    }

    void GraphicsDevice::Viewport(Xna::Viewport const& viewport) {
        impl->viewports[0] = viewport;
        impl->backend->SetViewport(viewport);
    }

    void GraphicsDevice::ScissorRectangle(Rectangle const& value) {
        impl->scissorRectangles[0] = value;
        //TODO: implementar ScissorRectangle
    }    

    void GraphicsDevice::Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) {
        //Platform::GraphicsDevice_Present(*this, rec, destination, overrideWindowHandle);
        impl->backend->Present(rec, destination, overrideWindowHandle);
    }
    
    void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) {
        impl->backend->Clear(options, color, depth, stencil);
    }    

    void GraphicsDevice::BlendState(Xna::BlendState const& value) { 
        const auto& currentBlendState = impl->backend->GetBlendState();

        if (value == currentBlendState)
            return;

        impl->backend->ApplyBlendState(value); 
    }

    void GraphicsDevice::BlendFactor(Color const& color) { 
        const auto& current = impl->backend->GetBlendFactor();

        if (color == current)
            return;

        impl->backend->ApplyBlendFactor(color); 
    }

    void GraphicsDevice::DepthStencilState(Xna::DepthStencilState const& value) { 
        const auto& current = impl->backend->GetDepthStencilState();

        if (value == current)
            return;

        impl->backend->ApplyDepthStencilState(value); 
    }

    void GraphicsDevice::RasterizerState(Xna::RasterizerState const& value) {
        const auto& current = impl->backend->GetRasterizerState();

        if (value == current)
            return;

        impl->backend->ApplyRasterizerState(value); 
    }
}