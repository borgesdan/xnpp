#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

namespace Xna {
	//
	// BlendState
	//	

	void Platform::BlendState_Apply(BlendState const& blendState, GraphicsDevice& device) {
		auto& impl = blendState.impl;

		//
		// Create desc
		//

		D3D11_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = DxHelpers::BlendToDx(impl->colorSourceBlend);
		desc.RenderTarget[0].DestBlend = DxHelpers::BlendToDx(impl->colorDestinationBlend);
		desc.RenderTarget[0].BlendOp = DxHelpers::BlendOperationToDx(impl->colorBlendFunction);
		desc.RenderTarget[0].SrcBlendAlpha = DxHelpers::BlendToDx(impl->alphaSourceBlend);
		desc.RenderTarget[0].DestBlendAlpha = DxHelpers::BlendToDx(impl->alphaDestinationBlend);
		desc.RenderTarget[0].BlendOpAlpha = DxHelpers::BlendOperationToDx(impl->alphaBlendFunction);
		desc.RenderTarget[0].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(impl->colorWriteChannels0);

		desc.RenderTarget[1].BlendEnable = true;
		desc.RenderTarget[1].SrcBlend = DxHelpers::BlendToDx(impl->colorSourceBlend);
		desc.RenderTarget[1].DestBlend = DxHelpers::BlendToDx(impl->colorDestinationBlend);
		desc.RenderTarget[1].BlendOp = DxHelpers::BlendOperationToDx(impl->colorBlendFunction);
		desc.RenderTarget[1].SrcBlendAlpha = DxHelpers::BlendToDx(impl->alphaSourceBlend);
		desc.RenderTarget[1].DestBlendAlpha = DxHelpers::BlendToDx(impl->alphaDestinationBlend);
		desc.RenderTarget[1].BlendOpAlpha = DxHelpers::BlendOperationToDx(impl->alphaBlendFunction);
		desc.RenderTarget[1].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(impl->colorWriteChannels1);

		desc.RenderTarget[2].BlendEnable = true;
		desc.RenderTarget[2].SrcBlend = DxHelpers::BlendToDx(impl->colorSourceBlend);
		desc.RenderTarget[2].DestBlend = DxHelpers::BlendToDx(impl->colorDestinationBlend);
		desc.RenderTarget[2].BlendOp = DxHelpers::BlendOperationToDx(impl->colorBlendFunction);
		desc.RenderTarget[2].SrcBlendAlpha = DxHelpers::BlendToDx(impl->alphaSourceBlend);
		desc.RenderTarget[2].DestBlendAlpha = DxHelpers::BlendToDx(impl->alphaDestinationBlend);
		desc.RenderTarget[2].BlendOpAlpha = DxHelpers::BlendOperationToDx(impl->alphaBlendFunction);
		desc.RenderTarget[2].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(impl->colorWriteChannels2);

		desc.RenderTarget[3].BlendEnable = true;
		desc.RenderTarget[3].SrcBlend = DxHelpers::BlendToDx(impl->colorSourceBlend);
		desc.RenderTarget[3].DestBlend = DxHelpers::BlendToDx(impl->colorDestinationBlend);
		desc.RenderTarget[3].BlendOp = DxHelpers::BlendOperationToDx(impl->colorBlendFunction);
		desc.RenderTarget[3].SrcBlendAlpha = DxHelpers::BlendToDx(impl->alphaSourceBlend);
		desc.RenderTarget[3].DestBlendAlpha = DxHelpers::BlendToDx(impl->alphaDestinationBlend);
		desc.RenderTarget[3].BlendOpAlpha = DxHelpers::BlendOperationToDx(impl->alphaBlendFunction);
		desc.RenderTarget[3].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(impl->colorWriteChannels3);

		//
		// Apply
		//
		auto& platform = impl->platformImpl;
		auto& deviceImpl = device.implGraphicsDevice->platformImpl;

		const auto hr = deviceImpl.device->CreateBlendState(&desc, platform.blendState.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("ID3D11Device.CreateBlendState() failed.");

		const auto sampleMask = static_cast<UINT>(impl->multiSampleMask);
		const auto color = impl->blendFactor.ToVector4();
		float blendFactor[4] = { color.X, color.Y, color.Z, color.W };

		deviceImpl.context->OMSetBlendState(platform.blendState.Get(), blendFactor, sampleMask);
	}

	//
	// DepthStencilState
	//

	void Platform::DepthStencilState_Apply(DepthStencilState const& depthStencil, GraphicsDevice& device) {
		auto& impl = depthStencil.impl;

		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = static_cast<BOOL>(impl->depthBufferEnable);
		desc.StencilEnable = static_cast<BOOL>(impl->stencilEnable);
		desc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>((int)impl->depthBufferFunction + 1);
		desc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>((int)impl->stencilFunction + 1);
		desc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>((int)impl->stencilPass + 1);
		desc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>((int)impl->stencilFail + 1);
		desc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>((int)impl->stencilDepthBufferFail + 1);

		if (!impl->twoSidedStencilMode) {
			desc.BackFace = desc.FrontFace;
		}
		else {
			desc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>((int)impl->counterClockwiseStencilFunction + 1);
			desc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>((int)impl->counterClockwiseStencilPass + 1);
			desc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>((int)impl->counterClockwiseStencilFail + 1);
			desc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>((int)impl->counterClockwiseStencilDepthBufferFail + 1);
		}

		constexpr UINT8 min = 0;
		UINT8 readMask = impl->stencilMask > static_cast<int32_t>(D3D11_DEFAULT_STENCIL_READ_MASK)
			? D3D11_DEFAULT_STENCIL_READ_MASK
			: std::max(min, static_cast<UINT8>(impl->stencilMask));

		UINT8 writeMask = impl->stencilWriteMask > static_cast<int32_t>(D3D11_DEFAULT_STENCIL_WRITE_MASK)
			? D3D11_DEFAULT_STENCIL_WRITE_MASK
			: std::max(min, static_cast<UINT8>(impl->stencilWriteMask));

		if (impl->stencilMask > static_cast<int32_t>(D3D11_DEFAULT_STENCIL_READ_MASK))
			readMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		else if (impl->stencilMask < 0)
			readMask = 0;

		desc.StencilReadMask = readMask;
		desc.StencilWriteMask = writeMask;
		desc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(impl->depthBufferWriteEnable);

		//
		// apply
		//

		auto& deviceImpl = device.implGraphicsDevice->platformImpl;
		const auto hr = deviceImpl.device->CreateDepthStencilState(&desc, impl->platformImpl.depthStencil.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("ID3D11Device.CreateDepthStencilState() failed.");

		const auto referenceStencil = static_cast<UINT>(impl->referenceStencil);
		deviceImpl.context->OMSetDepthStencilState(impl->platformImpl.depthStencil.Get(), referenceStencil);
	}

	//
	// RasterizerState
	//

	void Platform::RasterizerState_Apply(RasterizerState const& state, GraphicsDevice& device) {
		auto& impl = state.impl;

		D3D11_RASTERIZER_DESC desc{};
		desc.DepthClipEnable = true;
		desc.CullMode = DxHelpers::CullModeToDx(impl->cullMode);
		desc.FillMode = DxHelpers::FillModeToDx(impl->fillMode);
		desc.MultisampleEnable = static_cast<BOOL>(impl->multiSampleAntiAlias);
		desc.DepthBias = static_cast<INT>(impl->depthBias);
		desc.SlopeScaledDepthBias = static_cast<FLOAT>(impl->slopeScaleDepthBias);
		desc.ScissorEnable = static_cast<BOOL>(impl->scissorTestEnable);

		auto& deviceImpl = device.implGraphicsDevice->platformImpl;

		const auto hr = deviceImpl.device->CreateRasterizerState(
			&desc,
			state.impl->platformImpl.rasterizerState.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("ID3D11Device.CreateRasterizerState() failed.");

		deviceImpl.context->RSSetState(state.impl->platformImpl.rasterizerState.Get());
	}

	//
	// SamplerState
	//

	void Platform::SamplerState_Apply(SamplerState const& state, GraphicsDevice& device, size_t samplerIndex, SamplerStateApplyType type) {
		auto& impl = state.impl;
		auto& platform = impl->platformImpl;

		D3D11_SAMPLER_DESC desc{};
		desc.AddressU = DxHelpers::TextureAddresModeToDx(impl->addressU);
		desc.AddressV = DxHelpers::TextureAddresModeToDx(impl->addressV);
		desc.AddressW = DxHelpers::TextureAddresModeToDx(impl->addressW);
		desc.Filter = DxHelpers::TextureFilterToDx(impl->filter);
		desc.MaxAnisotropy = static_cast<UINT>(impl->maxAnisotropy);
		desc.MaxLOD = static_cast<FLOAT>(impl->maxMipLevel);
		desc.MinLOD = static_cast<FLOAT>(impl->minMipLevel);
		desc.MipLODBias = static_cast<FLOAT>(impl->mipMapLevelOfDetailBias);

		auto& deviceImpl = device.implGraphicsDevice->platformImpl;

		const auto hr = deviceImpl.device->CreateSamplerState(&desc, platform.samplerState.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("ID3D11Device.CreateSamplerState() failed.");

		if (type == SamplerStateApplyType::Pixel)
			deviceImpl.context->PSSetSamplers(static_cast<UINT>(samplerIndex), 1, platform.samplerState.GetAddressOf());
		else
			deviceImpl.context->VSSetSamplers(static_cast<UINT>(samplerIndex), 1, platform.samplerState.GetAddressOf());
	}
}

#endif