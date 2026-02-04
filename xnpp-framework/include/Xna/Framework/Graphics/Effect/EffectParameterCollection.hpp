#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTPARAMETERCOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTPARAMETERCOLLECTION_HPP

#include "Xna/CSharp/Collection.hpp"
#include "_EffectParameter.hpp"
#include <memory>

namespace Xna {
	class EffectParameterCollection {
	public:
		std::vector<EffectParameter>::iterator begin();
		std::vector<EffectParameter>::iterator end();
		std::vector<EffectParameter>::iterator begin() const;
		std::vector<EffectParameter>::iterator end() const;

		inline EffectParameterCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectParameterCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {

		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif