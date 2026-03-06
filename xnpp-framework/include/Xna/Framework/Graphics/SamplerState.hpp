#ifndef XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP

#include "Shared.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <optional>

namespace Xna {
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
		static constexpr const SamplerState& PointWrap();
		//Contains default state for point filtering and texture coordinate clamping.
		static constexpr const SamplerState& PointClamp();
		//Contains default state for linear filtering and texture coordinate wrapping.
		static constexpr const SamplerState& LinearWrap();
		//Contains default state for linear filtering and texture coordinate clamping.
		static constexpr const SamplerState& LinearClamp();
		//Contains default state for anisotropic filtering and texture coordinate wrapping.
		static constexpr const SamplerState& AnisotropicWrap();
		//Contains default state for anisotropic filtering and texture coordinate clamping.
		static constexpr const SamplerState& AnisotropicClamp();

		constexpr operator std::optional<SamplerState>() const noexcept { return std::make_optional<SamplerState>(*this); }
		constexpr operator std::optional<std::reference_wrapper<const SamplerState>>() const noexcept { return std::cref(*this); }

	private:
		constexpr SamplerState(TextureFilter filter, TextureAddressMode uvw )
			:Filter(filter), AddressU(uvw), AddressV(uvw), AddressW(uvw){ }

		friend class SamplerStateCollection;
		friend struct _SamplerState;
	};

	struct _SamplerState {		
		static constexpr SamplerState PointWrap = SamplerState(TextureFilter::Point, TextureAddressMode::Wrap);
		static constexpr SamplerState PointClamp = SamplerState(TextureFilter::Point, TextureAddressMode::Clamp);
		static constexpr SamplerState LinearWrap = SamplerState(TextureFilter::Linear, TextureAddressMode::Wrap);
		static constexpr SamplerState LinearClamp = SamplerState(TextureFilter::Linear, TextureAddressMode::Clamp);
		static constexpr SamplerState AnisotropicWrap = SamplerState(TextureFilter::Anisotropic, TextureAddressMode::Wrap);
		static constexpr SamplerState AnisotropicClamp = SamplerState(TextureFilter::Anisotropic, TextureAddressMode::Clamp);
	};
		
	constexpr const SamplerState& SamplerState::PointWrap() { return _SamplerState::PointWrap; }
	constexpr const SamplerState& SamplerState::PointClamp() { return _SamplerState::PointClamp; }
	constexpr const SamplerState& SamplerState::LinearWrap() { return _SamplerState::LinearWrap; }
	constexpr const SamplerState& SamplerState::LinearClamp() { return _SamplerState::LinearClamp; }
	constexpr const SamplerState& SamplerState::AnisotropicWrap() { return _SamplerState::AnisotropicWrap; }
	constexpr const SamplerState& SamplerState::AnisotropicClamp() { return _SamplerState::AnisotropicClamp; }

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