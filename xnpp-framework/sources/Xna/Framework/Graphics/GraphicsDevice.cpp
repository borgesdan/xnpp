#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
    GraphicsDevice::GraphicsDevice(GraphicsAdapter const& adapter, Xna::GraphicsProfile const& graphicsProfile, Xna::PresentationParameters const& presentationParameters) {
        impl = std::make_shared<Implementation>();
        impl->backend = PlatformNS::IGraphicsDevice::Create();

        impl->adapter = adapter;
        impl->graphicsProfile = graphicsProfile;
        impl->presentationParameters = presentationParameters;

        const auto maxSamplerStates = PlatformNS::Graphics_GetMaxSamplerStates();

        impl->samplerState = Xna::SamplerStateCollection(0, maxSamplerStates);
        impl->vertexSamplerState = Xna::SamplerStateCollection(0, maxSamplerStates);
        impl->blendState = Xna::BlendState::Opaque();
        impl->depthStencilState = Xna::DepthStencilState::Default();
        impl->rasterizerState = Xna::RasterizerState::CullCounterClockwise();

        impl->lazyInitialization = presentationParameters.DeviceWindowHandle == 0;

        CreateDevice(adapter, presentationParameters);

        if (impl->lazyInitialization)
            return;

        impl->backend->MakeWindowAssociation(presentationParameters);
    }

    void GraphicsDevice::BlendState(Xna::BlendState const& value) {        
        impl->blendState = value;
        impl->backend->ApplyBlendState(value);
    }

    void GraphicsDevice::BlendFactor(Color const& color) {
        impl->blendState.BlendFactor = color;
        impl->backend->ApplyBlendState(impl->blendState);
    }

    void GraphicsDevice::DepthStencilState(Xna::DepthStencilState const& value) {
        impl->depthStencilState = value;
        impl->backend->ApplyDepthStencilState(value);
    }

    void GraphicsDevice::MultiSampleMask(int32_t value) {
        impl->multiSampleMask = value;
        //TODO: MultiSampleMask não é aplicado no backend
    }

    void GraphicsDevice::RasterizerState(Xna::RasterizerState const& value) {
        impl->rasterizerState = value;
        impl->backend->ApplyRasterizerState(value);
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

    void GraphicsDevice::CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) {        
        impl->backend->CreateDevice(adapter, presentationParameters);
    }

    void GraphicsDevice::Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) {
        //Platform::GraphicsDevice_Present(*this, rec, destination, overrideWindowHandle);
        impl->backend->Present(rec, destination, overrideWindowHandle);
    }
    
    void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) {
        impl->backend->Clear(options, color, depth, stencil);
    }    
}