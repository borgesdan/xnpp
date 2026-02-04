#include "Xna/Framework/Graphics/SamplerState.hpp"

namespace Xna {	
	SamplerState SamplerState::PoinWrap() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Point;
		s.impl->addressU = TextureAddressMode::Wrap;
		s.impl->addressV = TextureAddressMode::Wrap;
		s.impl->addressW = TextureAddressMode::Wrap;
		return s;
	}

	SamplerState SamplerState::PointClamp() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Point;
		s.impl->addressU = TextureAddressMode::Clamp;
		s.impl->addressV = TextureAddressMode::Clamp;
		s.impl->addressW = TextureAddressMode::Clamp;
		return s;
	}

	SamplerState SamplerState::LinearWrap() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Linear;
		s.impl->addressU = TextureAddressMode::Wrap;
		s.impl->addressV = TextureAddressMode::Wrap;
		s.impl->addressW = TextureAddressMode::Wrap;
		return s;
	}

	SamplerState SamplerState::LinearClamp() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Linear;
		s.impl->addressU = TextureAddressMode::Clamp;
		s.impl->addressV = TextureAddressMode::Clamp;
		s.impl->addressW = TextureAddressMode::Clamp;
		return s;
	}

	SamplerState SamplerState::AnisotropicWrap() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Anisotropic;
		s.impl->addressU = TextureAddressMode::Wrap;
		s.impl->addressV = TextureAddressMode::Wrap;
		s.impl->addressW = TextureAddressMode::Wrap;
		return s;
	}

	SamplerState SamplerState::AnisotropicClamp() {
		auto s = SamplerState();
		s.impl->filter = TextureFilter::Anisotropic;
		s.impl->addressU = TextureAddressMode::Clamp;
		s.impl->addressV = TextureAddressMode::Clamp;
		s.impl->addressW = TextureAddressMode::Clamp;
		return s;
	}

	void SamplerStateCollection::ClearState(size_t index) {
		if (index >= samplers->size())
			return;

		samplers->erase(samplers->begin() + index);
	}

	void SamplerStateCollection::InitializeDeviceState(SamplerStateApplyType type) {
		for (size_t i = 0; i < samplers->size(); ++i) {
			auto& sampler = samplers->at(i);			
			sampler.Apply(*parentDevice, samplerOffset + i, type);
		}
	}
}