#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECTTECHNIQUE_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECTTECHNIQUE_HPP

#include <memory>
#include <optional>
#include <string>

namespace Xna {
	struct EffectPassCollection;
	struct EffectAnnotationCollection;
	class Effect;

	class EffectTechnique final {
	public:
		inline std::string Name() const;
		inline EffectPassCollection Passes() const;
		inline EffectAnnotationCollection Annotations() const;

		inline EffectTechnique(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectTechnique const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		EffectTechnique(Effect const& effect);

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		friend class Effect;
	};
}

#endif