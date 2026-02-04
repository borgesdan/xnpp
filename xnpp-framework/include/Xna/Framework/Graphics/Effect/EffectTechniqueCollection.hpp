#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTTECHNIQUECOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTTECHNIQUECOLLECTION_HPP

#include "_EffectTechnique.hpp"
#include "_Effect.hpp"
#include <memory>
#include <vector>

namespace Xna {
	class EffectTechniqueCollection final {
	public:
		inline size_t Count() const {return impl->techniques.size(); }

		EffectTechnique operator[](size_t index);
		EffectTechnique operator[](std::string const& name);

		inline std::vector<EffectTechnique>::iterator begin() { return impl->techniques.begin(); }
		inline std::vector<EffectTechnique>::iterator end() { return impl->techniques.end(); }
		inline std::vector<EffectTechnique>::iterator begin() const { return impl->techniques.begin(); }
		inline std::vector<EffectTechnique>::iterator end() const { return impl->techniques.end(); }

		inline EffectTechniqueCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectTechniqueCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:		
		EffectTechniqueCollection(Effect const& effect, size_t count);

		struct Implementation {
			std::vector<EffectTechnique> techniques;
			std::optional<Effect> parent;
		};

		std::shared_ptr<Implementation> impl;

		friend class Effect;
	};
}

#endif