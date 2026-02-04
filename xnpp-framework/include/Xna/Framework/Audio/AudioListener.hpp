#ifndef XNA_FRAMEWORK_AUDIO_AUDIOLISTER_HPP
#define XNA_FRAMEWORK_AUDIO_AUDIOLISTER_HPP

#include "UnsafeNativeStructures.hpp"
#include "Xna/Framework/Vector3.hpp"

namespace Xna {
	class AudioListener {
	public:
		inline AudioListener()
		{
			listenerData._Position = Vector3::Zero();
			listenerData._Velocity = Vector3::Zero();
			listenerData._Forward = UnsafeNativeStructures::FlipHandedness(Vector3::Forward());
			listenerData._Up = UnsafeNativeStructures::FlipHandedness(Vector3::Up());
		}

		constexpr void Position(Vector3 const& value) {
			listenerData._Position = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Velocity() const {
			return UnsafeNativeStructures::FlipHandedness(listenerData._Velocity);
		}

		constexpr void Velocity(Vector3 const& value) {
			listenerData._Velocity = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Forward() const {
			return UnsafeNativeStructures::FlipHandedness(listenerData._Forward);
		}

		constexpr void Forward(Vector3 const& value) {
			listenerData._Forward = UnsafeNativeStructures::FlipHandedness(value);
		}

		constexpr Vector3 Up() const {
			return UnsafeNativeStructures::FlipHandedness(listenerData._Up);
		}

		constexpr void Up(Vector3 const& value) {
			listenerData._Up = UnsafeNativeStructures::FlipHandedness(value);
		}

		UnsafeNativeStructures::XACT_LISTENER_DATA& GetData() {
			return listenerData;
		}

		const UnsafeNativeStructures::XACT_LISTENER_DATA& GetData() const {
			return listenerData;
		}

	private:
		UnsafeNativeStructures::XACT_LISTENER_DATA listenerData;
	};
}

#endif