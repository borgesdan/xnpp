#ifndef XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_SAMPLERSTATE_HPP

#include "Shared.hpp"
#include "GraphicsResource.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <optional>
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	enum class SamplerStateApplyType {
		Pixel,
		Vertex,
	};

	//Contains sampler state, which determines how to sample texture data. 
	class SamplerState final : public GraphicsResource {
	public:		
		inline SamplerState() { impl = std::make_shared<Implementation>(); }

		~SamplerState() override = default;

		//Gets or sets the maximum anisotropy. The default value is 0.
		inline void MaxAnisotropy(uint32_t value) { impl->maxAnisotropy = value; }
		//Gets or sets the maximum anisotropy. The default value is 0.
		inline uint32_t MaxAnisotropy() const { return impl->maxAnisotropy; }
		//Gets or sets the type of filtering during sampling.
		inline void Filter(TextureFilter value) { impl->filter = value; }
		//Gets or sets the type of filtering during sampling.
		inline TextureFilter Filter() const { return impl->filter; }
		////Gets or sets the texture-address mode for the u-coordinate.		
		//Gets or sets the texture-address mode for the u-coordinate.
		inline TextureAddressMode AddressU() const { return impl->addressU; }
		//Gets or sets the texture-address mode for the v-coordinate.
		inline TextureAddressMode AddressV() const { return impl->addressV; }
		//Gets or sets the texture-address mode for the w-coordinate.
		inline TextureAddressMode AddressW() const { return impl->addressW; }
		////Gets or sets the texture-address mode for the u-coordinate.
		inline void AddressU(TextureAddressMode value) { impl->addressU = value; }
		//Gets or sets the texture-address mode for the v-coordinate.
		inline void AddressV(TextureAddressMode value) { impl->addressV = value; }
		////Gets or sets the texture-address mode for the w-coordinate.
		inline void AddressW(TextureAddressMode value) { impl->addressW = value; }		

		//Gets or sets the mipmap LOD bias. The default value is 0. 
		inline void MipMapLevelOfDetailBias(float value) { impl->mipMapLevelOfDetailBias = value; }
		//Gets or sets the mipmap LOD bias. The default value is 0. 
		inline float MipMapLevelOfDetailBias() const { return impl->mipMapLevelOfDetailBias; }

		//Gets or sets the level of detail (LOD) index of the largest map to use. 
		inline float MaxMipLevel() const { return impl->maxMipLevel; }
		//Gets or sets the level of detail (LOD) index of the largest map to use. 
		inline void MaxMipLevel(float value) { impl->maxMipLevel = value; }

		//Gets or sets the level of detail (LOD) index of the smaller map to use. 
		inline void MinMipLevel(float value) { impl->minMipLevel = value; }
		//Gets or sets the level of detail (LOD) index of the smaller map to use. 
		inline float MinMipLevel() const {	return impl->minMipLevel; }

		//Contains default state for point filtering and texture coordinate wrapping.
		XNPP_API static SamplerState PoinWrap();
		//Contains default state for point filtering and texture coordinate clamping.
		XNPP_API static SamplerState PointClamp();
		//Contains default state for linear filtering and texture coordinate wrapping.
		XNPP_API static SamplerState LinearWrap();
		//Contains default state for linear filtering and texture coordinate clamping.
		XNPP_API static SamplerState LinearClamp();
		//Contains default state for anisotropic filtering and texture coordinate wrapping.
		XNPP_API static SamplerState AnisotropicWrap();
		//Contains default state for anisotropic filtering and texture coordinate clamping.
		XNPP_API static SamplerState AnisotropicClamp();

		inline SamplerState(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(SamplerState const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			int32_t maxAnisotropy{};
			TextureFilter filter{};
			TextureAddressMode addressU{};
			TextureAddressMode addressV{};
			TextureAddressMode addressW{};
			float mipMapLevelOfDetailBias{};
			float maxMipLevel{};
			float minMipLevel{};

			PlatformImpl::SamplerStateImpl platformImpl{};
		};		
	
		inline void Apply(GraphicsDevice& device, size_t samplerIndex, SamplerStateApplyType type) { Platform::SamplerState_Apply(*this, device, samplerIndex, type); };
		std::shared_ptr<Implementation> impl;

		friend class SamplerStateCollection;
		friend struct Platform;
	};

	//Collection of SamplerState objects. 
	class SamplerStateCollection {
	public:
		//Gets a SamplerState object in the collection at the specified index value.
		SamplerState& operator[](size_t index);
		inline size_t Count() const { return samplers->size(); }

		inline auto begin() { return samplers->begin(); }
		inline auto end() { return samplers->end(); }
		inline auto begin() const { return samplers->begin(); }
		inline auto end() const { return samplers->end(); }

	private:
		friend class GraphicsDevice;

		SamplerStateCollection() = default;
		inline SamplerStateCollection(GraphicsDevice parent, size_t samplerOffset, size_t maxSamplers) : parentDevice(parent), samplerOffset(samplerOffset) {
			samplers = std::make_shared<std::vector<SamplerState>>(maxSamplers); //TODO revisar			
		}
		void ClearState(size_t index);

		void InitializeDeviceState(SamplerStateApplyType type);

		std::optional<GraphicsDevice> parentDevice;
		size_t samplerOffset{0};
		std::shared_ptr<std::vector<SamplerState>> samplers;

		friend class GraphicsDevice;
	};
}

#endif