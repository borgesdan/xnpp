#ifndef XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP

#include "Shared.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <optional>
#include "_GraphicsDevice.hpp"

namespace Xna {
	enum class SamplerStateApplyType {
		Pixel,
		Vertex,
	};

	//Contains sampler state, which determines how to sample texture data. 
	struct SamplerState {
	public:
		constexpr SamplerState() = default;

		//Gets or sets the maximum anisotropy. The default value is 0.
		uint32_t MaxAnisotropy{ 0 };
		//Gets or sets the type of filtering during sampling.
		TextureFilter Filter{ TextureFilter::Linear };
		//Gets or sets the texture-address mode for the u-coordinate.
		TextureAddressMode AddressU{ TextureAddressMode::Wrap };
		//Gets or sets the texture-address mode for the v-coordinate.
		TextureAddressMode AddressV{ TextureAddressMode::Wrap };
		//Gets or sets the texture-address mode for the w-coordinate.
		TextureAddressMode AddressW{ TextureAddressMode::Wrap };
		//Gets or sets the mipmap LOD bias. The default value is 0. 
		float MipMapLevelOfDetailBias{ 0 };
		//Gets or sets the level of detail (LOD) index of the largest map to use. 
		float MaxMipLevel{ 0 };
		//Gets or sets the level of detail (LOD) index of the smaller map to use. 
		float MinMipLevel{ 0 };

		//Contains default state for point filtering and texture coordinate wrapping.
		static SamplerState PoinWrap() { return SamplerState(TextureFilter::Point, TextureAddressMode::Clamp); }
		//Contains default state for point filtering and texture coordinate clamping.
		static SamplerState PointClamp() { return SamplerState(TextureFilter::Point, TextureAddressMode::Clamp); }
		//Contains default state for linear filtering and texture coordinate wrapping.
		static SamplerState LinearWrap() { return SamplerState(TextureFilter::Linear, TextureAddressMode::Wrap); }
		//Contains default state for linear filtering and texture coordinate clamping.
		static SamplerState LinearClamp() { return SamplerState(TextureFilter::Linear, TextureAddressMode::Clamp); }
		//Contains default state for anisotropic filtering and texture coordinate wrapping.
		static SamplerState AnisotropicWrap() { return SamplerState(TextureFilter::Anisotropic, TextureAddressMode::Wrap); }
		//Contains default state for anisotropic filtering and texture coordinate clamping.
		static SamplerState AnisotropicClamp() { return SamplerState(TextureFilter::Anisotropic, TextureAddressMode::Clamp); }

	private:
		constexpr SamplerState(TextureFilter filter, TextureAddressMode uvw )
			:Filter(filter), AddressU(uvw), AddressV(uvw), AddressW(uvw){ }

		int32_t maxAnisotropy{};
		TextureFilter filter{};
		TextureAddressMode addressU{};
		TextureAddressMode addressV{};
		TextureAddressMode addressW{};
		float mipMapLevelOfDetailBias{};
		float maxMipLevel{};
		float minMipLevel{};

		friend class SamplerStateCollection;
	};

	//Collection of SamplerState objects. 
	class SamplerStateCollection {
	public:
		//Gets a SamplerState object in the collection at the specified index value.
		std::optional<SamplerState>& operator[](size_t index) {	return impl->samplers.at(index); }
		size_t Count() const { return impl->samplers.size(); }
		auto begin() { return impl->samplers.begin(); }
		auto end() { return impl->samplers.end(); }
		auto begin() const { return impl->samplers.begin(); }
		auto end() const { return impl->samplers.end(); }

		inline SamplerStateCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(SamplerStateCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		SamplerStateCollection() = default;
		SamplerStateCollection(size_t samplerOffset, size_t maxSamplers) {
			impl = std::make_shared<Implementation>();
			impl->samplerOffset = samplerOffset;
			impl->samplers.resize(maxSamplers);
		}

		struct Implementation {
			size_t samplerOffset{ 0 };
			std::vector<std::optional<SamplerState>> samplers;
		};

		std::shared_ptr<Implementation> impl;

		friend class GraphicsDevice;
	};
}

#endif