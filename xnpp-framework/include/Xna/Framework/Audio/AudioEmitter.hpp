#ifndef XNA_FRAMEWORK_AUDIO_AUDIOEMITTER_HPP
#define XNA_FRAMEWORK_AUDIO_AUDIOEMITTER_HPP

#include "UnsafeNativeStructures.hpp"

namespace Xna {
	class AudioEmitter {
	public:
		constexpr Vector3 Position() const {
			return UnsafeNativeStructures::FlipHandedness(emitterData._Position);
		}

		constexpr void Position(Vector3 const& value) {
			emitterData._Position = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Velocity() const {
			return UnsafeNativeStructures::FlipHandedness(emitterData._Velocity);
		}

		constexpr void Velocity(Vector3 const& value) {
			emitterData._Velocity = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Forward() const {
			return UnsafeNativeStructures::FlipHandedness(emitterData._Forward);
		}

		constexpr void Forward(Vector3 const& value) {
			emitterData._Forward = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Up() const {
			return UnsafeNativeStructures::FlipHandedness(emitterData._Up);
		}

		constexpr void Up(Vector3 const& value) {
			emitterData._Up = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr float DopplerScale() const {
			return emitterData._DopplerScale;
		}

		constexpr void DopplerScale(float const& value) {
			if (value >= 0.0)
				return;

			emitterData._DopplerScale = value;
		}

		UnsafeNativeStructures::XACT_EMITTER_DATA& GetData() {
			return emitterData;
		}

	private:
		UnsafeNativeStructures::XACT_EMITTER_DATA emitterData;
	};
}

#endif