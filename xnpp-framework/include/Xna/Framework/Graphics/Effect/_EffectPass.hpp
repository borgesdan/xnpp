#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECTPASS_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECTPASS_HPP

#include <memory>
#include <string>
#include <optional>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class EffectAnnotationCollection;
	class EffectTechnique;

	class EffectPass final {
	public:
		inline std::string Name() const;
		inline EffectAnnotationCollection Annotations() const;
		XNPP_API void Apply();

		inline EffectPass(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectPass const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		XNPP_API EffectPass(EffectTechnique const& technique);

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};
}

#endif