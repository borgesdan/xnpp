#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
    GraphicsDevice::GraphicsDevice(GraphicsAdapter const& adapter, Xna::GraphicsProfile const& graphicsProfile, Xna::PresentationParameters const& presentationParameters) {
        implGraphicsDevice = std::make_shared<Implementation>();
        implGraphicsDevice->adapter = adapter;
        implGraphicsDevice->graphicsProfile = graphicsProfile;
        implGraphicsDevice->presentationParameters = presentationParameters;
        implGraphicsDevice->samplerState = Xna::SamplerStateCollection(*this, 0, Platform::MaxSamplerStates);
        implGraphicsDevice->vertexSamplerState = Xna::SamplerStateCollection(*this, 0, Platform::MaxSamplerStates);
        implGraphicsDevice->blendState = Xna::BlendState::Opaque();
        implGraphicsDevice->depthStencilState = Xna::DepthStencilState::Default();
        implGraphicsDevice->rasterizerState = Xna::RasterizerState::CullCounterClockwise();

        implGraphicsDevice->lazyInitialization = presentationParameters.DeviceWindowHandle == 0;

        CreateDevice(adapter, presentationParameters);

        if (!implGraphicsDevice->lazyInitialization)
        {
            Platform::GraphicsDevice_MakeWindowAssociation(*this, presentationParameters);
            implGraphicsDevice->current2DRenderTargets[0] = CreateBackBufferRenderTarget();
        }
        
        Viewport(Xna::Viewport(0, 0, presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight, 0, 1));
        
        implGraphicsDevice->blendState->Apply(*this);
        implGraphicsDevice->depthStencilState->Apply(*this);
        implGraphicsDevice->rasterizerState->Apply(*this);
        implGraphicsDevice->samplerState.InitializeDeviceState(SamplerStateApplyType::Pixel);
        implGraphicsDevice->vertexSamplerState.InitializeDeviceState(SamplerStateApplyType::Vertex);
                
        Platform::GraphicsDevice_SetRenderTargets(*this);
        Platform::GraphicsDevice_GetScissorRectangles(*this, implGraphicsDevice->scissorRectangles);
    }

    void GraphicsDevice::BlendState(Xna::BlendState const& value) {        
        implGraphicsDevice->blendState = value;
        implGraphicsDevice->blendState->Apply(*this);
    }

    void GraphicsDevice::BlendFactor(Color const& color) {
        implGraphicsDevice->blendState->impl->blendFactor = color;
        implGraphicsDevice->blendState->Apply(*this);
    }

    void GraphicsDevice::DepthStencilState(Xna::DepthStencilState const& value) {
        implGraphicsDevice->depthStencilState = value;
        implGraphicsDevice->depthStencilState->Apply(*this);
    }

    void GraphicsDevice::MultiSampleMask(int32_t value) {
        implGraphicsDevice->multiSampleMask = value;
    }

    void GraphicsDevice::RasterizerState(Xna::RasterizerState const& value) {
        implGraphicsDevice->rasterizerState = value;
        implGraphicsDevice->rasterizerState->Apply(*this);
    }

    void GraphicsDevice::ReferenceStencil(int32_t value) {
        implGraphicsDevice->referenceStencil = value;
    }

    void GraphicsDevice::Viewport(Xna::Viewport const& viewport) {
        implGraphicsDevice->viewports[0] = viewport;
        Platform::GraphicsDevice_SetViewport(*this, viewport);
    }

    void GraphicsDevice::ScissorRectangle(Rectangle const& value) {
        implGraphicsDevice->scissorRectangles[0] = value;
        Platform::GraphicsDevice_SetScissorRectangles(*this, implGraphicsDevice->scissorRectangles);
    }

    void GraphicsDevice::CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) {        
        Platform::GraphicsDevice_CreateDevice(*this, adapter, presentationParameters);               
    }

    void GraphicsDevice::Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) {
        Platform::GraphicsDevice_Present(*this, rec, destination, overrideWindowHandle);
        Platform::GraphicsDevice_SetRenderTargets(*this);
    }
    
    void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) {
        switch (options)
        {
        case Xna::ClearOptions::Target:
        {
            Platform::GraphicsDevice_ClearRenderTarget(*this, color);
            break;
        }
        case Xna::ClearOptions::DepthBuffer:
        {
            throw CSharp::NotSupportedException("TODO: implementar");
            //implementation->context->ClearDepthStencilView(nullptr, 0, 0, 0);
            break;
        }
        case Xna::ClearOptions::Stencil:
        {
            throw CSharp::NotSupportedException("TODO: implementar");
            //implementation->context->ClearDepthStencilView(nullptr, 0, 0, 0);
            break;
        }
        default:
            break;
        }
    }    
}