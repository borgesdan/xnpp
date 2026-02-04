#ifndef XNA_PLATFORM_WINDOWSPLATFORMHELPERS_HPP
#define XNA_PLATFORM_WINDOWSPLATFORMHELPERS_HPP

#ifdef PLATFORM_WINDOWS

#include "WindowsHeaders.hpp"
#include "Xna/Framework/Rectangle.hpp"
#include "Xna/Framework/Graphics/Shared.hpp"
#include "Xna/Framework/Graphics/Viewport.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/IO/Stream.hpp"
#include "Xna/Framework/Matrix.hpp"

namespace Xna {
	struct WindowsHelpers {
		static std::string LPWSTRToString(LPWSTR lpwstr);
	};

	struct DxHelpers {
		static inline UINT BytesPerPixelFromDXGIFormat(DXGI_FORMAT fmt)
		{
			switch (fmt)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return 4;

			case DXGI_FORMAT_R16G16B16A16_FLOAT:
			case DXGI_FORMAT_R16G16B16A16_UINT:
				return 8;

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
			case DXGI_FORMAT_R32G32B32A32_UINT:
				return 16;

			default:
				throw CSharp::InvalidOperationException("Formato não suportado.");
			}
		}

		static Microsoft::WRL::ComPtr<IStream> CreateIStreamFromCustomStream(CSharp::Stream& stream);
		static void CopyIStreamToCustomStream(IStream* iStream, CSharp::Stream& stream);

		static constexpr DXGI_FORMAT SurfaceFormatToDx(SurfaceFormat format)
		{
			switch (format)
			{
			case SurfaceFormat::Color://21                
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case SurfaceFormat::Bgr565: //23                
				return DXGI_FORMAT_B5G6R5_UNORM;
			case SurfaceFormat::Bgra5551://25                
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			case SurfaceFormat::Bgra4444://26                
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			case SurfaceFormat::Dxt1://827611204                
				return DXGI_FORMAT_BC1_UNORM;
			case SurfaceFormat::Dxt3://861165636                
				return DXGI_FORMAT_BC2_UNORM;
			case SurfaceFormat::Dxt5://894720068                
				return DXGI_FORMAT_BC3_UNORM;
			case SurfaceFormat::NormalizedByte2://60                
				return DXGI_FORMAT_R8G8_SNORM;
			case SurfaceFormat::NormalizedByte4://63                
				return DXGI_FORMAT_R8G8B8A8_SNORM;
			case SurfaceFormat::Rgba1010102://31                
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			case SurfaceFormat::Rg32://34                
				return DXGI_FORMAT_R16G16_UNORM;
			case SurfaceFormat::Rgba64://36                
				return DXGI_FORMAT_R16G16B16A16_UNORM;
			case SurfaceFormat::Alpha8://28
				return DXGI_FORMAT_A8_UNORM;
			case SurfaceFormat::Single://114                
				return DXGI_FORMAT_R32_FLOAT;
			case SurfaceFormat::Vector2://115                
				return DXGI_FORMAT_R32G32_FLOAT;
			case SurfaceFormat::Vector4://116                
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case SurfaceFormat::HalfSingle://111                
				return DXGI_FORMAT_R16_FLOAT;
			case SurfaceFormat::HalfVector2://112                
				return DXGI_FORMAT_R16G16_FLOAT;
			case SurfaceFormat::HalfVector4://113                
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case SurfaceFormat::HdrBlendable://113                
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			default://0                
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		static DXGI_FORMAT DepthFormatToDx(DepthFormat format)
		{
			switch (format)
			{
			case DepthFormat::Depth16:   return DXGI_FORMAT_D16_UNORM;
			case DepthFormat::Depth24:   return DXGI_FORMAT_D24_UNORM_S8_UINT;			
			case DepthFormat::Depth24Stencil8:   return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case DepthFormat::Depth32: return DXGI_FORMAT_D32_FLOAT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}

		static DepthFormat DepthFormatToXna(DXGI_FORMAT format) {
			switch (format)
			{
			case DXGI_FORMAT_D16_UNORM:
				return DepthFormat::Depth16;
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
				return DepthFormat::Depth24Stencil8;
			case DXGI_FORMAT_D32_FLOAT:
				return DepthFormat::Depth32;
			default:
				return DepthFormat::Depth32;
			}
		}

		static constexpr SurfaceFormat SurfaceFormatToXna(DXGI_FORMAT format) {
			switch (format)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return SurfaceFormat::Color;
			case DXGI_FORMAT_B5G6R5_UNORM:
				return SurfaceFormat::Bgr565;
			case DXGI_FORMAT_B5G5R5A1_UNORM:
				return SurfaceFormat::Bgra5551;
			case DXGI_FORMAT_B4G4R4A4_UNORM:
				return SurfaceFormat::Bgra4444;
			case DXGI_FORMAT_BC2_UNORM:
				return SurfaceFormat::Dxt3;
			case DXGI_FORMAT_BC3_UNORM:
				return SurfaceFormat::Dxt5;
			case DXGI_FORMAT_R8G8_SNORM:
				return  SurfaceFormat::NormalizedByte2;
			case DXGI_FORMAT_R8G8B8A8_SNORM:
				return SurfaceFormat::NormalizedByte4;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				return SurfaceFormat::Rgba1010102;
			case DXGI_FORMAT_R16G16_UNORM:
				return SurfaceFormat::Rg32;
			case DXGI_FORMAT_R16G16B16A16_UNORM:
				return SurfaceFormat::Rgba64;
			case DXGI_FORMAT_A8_UNORM:
				return SurfaceFormat::Alpha8;
			case DXGI_FORMAT_R32_FLOAT:
				return SurfaceFormat::Single;
			case DXGI_FORMAT_R32G32_FLOAT:
				return SurfaceFormat::Vector2;
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				return SurfaceFormat::Vector4;
			case DXGI_FORMAT_R16_FLOAT:
				return SurfaceFormat::HalfSingle;
			case DXGI_FORMAT_R16G16_FLOAT:
				return SurfaceFormat::HalfVector2;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				return SurfaceFormat::HalfVector4;
			default:
				return SurfaceFormat::Unknown;
			}
		}

		static constexpr Blend BlendToXna(D3D11_BLEND blend) {
			switch (blend) {
			case D3D11_BLEND_ZERO:
				return Blend::Zero;
			case D3D11_BLEND_ONE:
				return Blend::One;
			case D3D11_BLEND_SRC_COLOR:
				return Blend::SourceColor;
			case D3D11_BLEND_INV_SRC_COLOR:
				return Blend::InverseSourceColor;
			case D3D11_BLEND_SRC_ALPHA:
				return Blend::SourceAlpha;
			case D3D11_BLEND_INV_SRC_ALPHA:
				return Blend::InverseSourceAlpha;
			case D3D11_BLEND_DEST_ALPHA:
				return Blend::DestinationAlpha;
			case D3D11_BLEND_INV_DEST_ALPHA:
				return Blend::InverseDestinationAlpha;
			case D3D11_BLEND_DEST_COLOR:
				return Blend::DestinationColor;
			case D3D11_BLEND_INV_DEST_COLOR:
				return Blend::InverseDestinationColor;
			case D3D11_BLEND_SRC_ALPHA_SAT:
				return Blend::SourceAlphaSaturation;
			case D3D11_BLEND_BLEND_FACTOR:
				return Blend::BlendFactor;
			case D3D11_BLEND_INV_BLEND_FACTOR:
				return Blend::InverseBlendFactor;
			case D3D11_BLEND_SRC1_COLOR:
				return Blend::Source1Color;
			case D3D11_BLEND_INV_SRC1_COLOR:
				return Blend::InverseSource1Color;
			case D3D11_BLEND_SRC1_ALPHA:
				return Blend::Source1Alpha;
			case D3D11_BLEND_INV_SRC1_ALPHA:
				return Blend::InverseSource1Alpha;
			default:
				return Blend::Zero;
			}
		}

		static constexpr D3D11_BLEND BlendToDx(Blend blend) {
			switch (blend)
			{
			case Xna::Blend::Zero:
				return D3D11_BLEND_ZERO;
			case Xna::Blend::One:
				return D3D11_BLEND_ONE;
			case Xna::Blend::SourceColor:
				return D3D11_BLEND_SRC_COLOR;
			case Xna::Blend::InverseSourceColor:
				return D3D11_BLEND_INV_SRC_COLOR;
			case Xna::Blend::SourceAlpha:
				return D3D11_BLEND_SRC_ALPHA;
			case Xna::Blend::InverseSourceAlpha:
				return D3D11_BLEND_INV_SRC_ALPHA;
			case Xna::Blend::DestinationAlpha:
				return D3D11_BLEND_DEST_ALPHA;
			case Xna::Blend::InverseDestinationAlpha:
				return D3D11_BLEND_INV_DEST_ALPHA;
			case Xna::Blend::DestinationColor:
				return D3D11_BLEND_DEST_COLOR;
			case Xna::Blend::InverseDestinationColor:
				return D3D11_BLEND_INV_DEST_COLOR;
			case Xna::Blend::SourceAlphaSaturation:
				return D3D11_BLEND_SRC_ALPHA_SAT;
			case Xna::Blend::BlendFactor:
				return D3D11_BLEND_BLEND_FACTOR;
			case Xna::Blend::InverseBlendFactor:
				return D3D11_BLEND_INV_BLEND_FACTOR;
			case Xna::Blend::Source1Color:
				return D3D11_BLEND_SRC1_COLOR;
			case Xna::Blend::InverseSource1Color:
				return D3D11_BLEND_INV_SRC1_COLOR;
			case Xna::Blend::Source1Alpha:
				return D3D11_BLEND_SRC1_ALPHA;
			case Xna::Blend::InverseSource1Alpha:
				return D3D11_BLEND_INV_SRC1_ALPHA;
			default:
				return D3D11_BLEND_ZERO;
			}
		}

		static constexpr D3D11_BLEND_OP BlendOperationToDx(BlendOperation op) {
			return static_cast<D3D11_BLEND_OP>(static_cast<int>(op) + 1);
		}

		static constexpr BlendOperation BlendOperationToXna(D3D11_BLEND_OP op) {
			return static_cast<BlendOperation>(static_cast<int>(op) - 1);
		}

		static constexpr D3D11_COLOR_WRITE_ENABLE ColorWriteChannelsToDx(ColorWriteChannels colorWrite) {
			switch (colorWrite)
			{
			case Xna::ColorWriteChannels::Red:
				return D3D11_COLOR_WRITE_ENABLE_RED;
			case Xna::ColorWriteChannels::Green:
				return D3D11_COLOR_WRITE_ENABLE_GREEN;
			case Xna::ColorWriteChannels::Blue:
				return D3D11_COLOR_WRITE_ENABLE_BLUE;
			case Xna::ColorWriteChannels::Alpha:
				return D3D11_COLOR_WRITE_ENABLE_ALPHA;
			case Xna::ColorWriteChannels::All:
				return D3D11_COLOR_WRITE_ENABLE_ALL;
			default:
				return D3D11_COLOR_WRITE_ENABLE_ALL;
			}
		}

		static constexpr ColorWriteChannels ColorWriteChannelsToXna(D3D11_COLOR_WRITE_ENABLE colorWrite) {
			switch (colorWrite) {
			case D3D11_COLOR_WRITE_ENABLE_RED:
				return Xna::ColorWriteChannels::Red;
			case D3D11_COLOR_WRITE_ENABLE_GREEN:
				return Xna::ColorWriteChannels::Green;
			case D3D11_COLOR_WRITE_ENABLE_BLUE:
				return Xna::ColorWriteChannels::Blue;
			case D3D11_COLOR_WRITE_ENABLE_ALPHA:
				return Xna::ColorWriteChannels::Alpha;
			case D3D11_COLOR_WRITE_ENABLE_ALL:
				return Xna::ColorWriteChannels::All;
			default:
				return Xna::ColorWriteChannels::All;
			}
		}

		static constexpr D3D11_TEXTURE_ADDRESS_MODE TextureAddresModeToDx(TextureAddressMode value) {
			return static_cast<D3D11_TEXTURE_ADDRESS_MODE>(static_cast<int>(value) + 1);
		}

		static constexpr TextureAddressMode TextureAddresModeToXna(D3D11_TEXTURE_ADDRESS_MODE value) {
			return static_cast<TextureAddressMode>(value - 1);
		}

		static constexpr D3D11_FILTER TextureFilterToDx(TextureFilter value) {
			switch (value)
			{
			case TextureFilter::Linear:
				return D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case TextureFilter::Point:
				return D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			case TextureFilter::Anisotropic:
				return D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
			case TextureFilter::LinearMipPoint:
				return D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case TextureFilter::PointMipLinear:
				return D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			case TextureFilter::MinLinearMagPointMipLinear:
				return D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case TextureFilter::MinLinearMagPointMipPoint:
				return D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case TextureFilter::MinPointMagLinearMipLinear:
				return D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			case TextureFilter::MinPointMagLinearMipPoint:
				return D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			default:
				return D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}

		static constexpr D3D11_VIEWPORT ViewportToDx(Viewport const& value) {
			D3D11_VIEWPORT _view{};
			_view.TopLeftX = value.X;
			_view.TopLeftY = value.Y;
			_view.Width = value.Width;
			_view.Height = value.Height;
			_view.MinDepth = value.MinDepth;
			_view.MaxDepth = value.MaxDepth;

			return _view;
		}

		static constexpr D3D11_FILL_MODE FillModeToDx(FillMode value) {
			if (value == FillMode::WireFrame)
				return D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

			return D3D11_FILL_MODE::D3D11_FILL_SOLID;
		}

		static constexpr D3D11_CULL_MODE CullModeToDx(CullMode value) {
			switch (value) {
			case CullMode::None:
				return D3D11_CULL_NONE;
			case CullMode::CullClockwiseFace:
				return D3D11_CULL_FRONT;
			case CullMode::CullCounterClockwiseFace:
				return D3D11_CULL_BACK;
			default:
				return D3D11_CULL_NONE;
			}
		}

		static constexpr DirectX::XMMATRIX MatrixToDx(Matrix const& value) {
			auto m = DirectX::XMMATRIX(
				value.M11,
				value.M12,
				value.M13,
				value.M14,
				value.M21,
				value.M22,
				value.M23,
				value.M24,
				value.M31,
				value.M32,
				value.M33,
				value.M34,
				value.M41,
				value.M42,
				value.M43,
				value.M44
			);

			return m;
		}			

		static constexpr RECT RectangleToDx(Rectangle const& rect) {
			RECT r{};
			r.left = rect.Left();
			r.top = rect.Top();
			r.right = rect.Right();
			r.bottom = rect.Bottom();

			return r;
		}

		static constexpr DirectX::SpriteEffects SpriteEffectsToDx(SpriteEffects e) {
			return static_cast<DirectX::SpriteEffects>(e);
		}

		static constexpr DirectX::SpriteSortMode SpriteSortModeToDx(SpriteSortMode ssm) {
			return static_cast<DirectX::SpriteSortMode>(ssm);
		}
	};
}

#endif
#endif
