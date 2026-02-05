#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTANNOTATIONCOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTANNOTATIONCOLLECTION_HPP

#include <memory>
#include <vector>
#include "_EffectAnnotation.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class EffectAnnotationCollection final{
	public:
		inline size_t Count() const { return impl->annotations.size(); }

		XNPP_API EffectAnnotation operator[](size_t index);
		XNPP_API EffectAnnotation operator[](std::string const& name);

		inline std::vector<EffectAnnotation>::iterator begin() { return impl->annotations.begin(); }
		inline std::vector<EffectAnnotation>::iterator end() { return impl->annotations.end(); }
		inline std::vector<EffectAnnotation>::iterator begin() const { return impl->annotations.begin(); }
		inline std::vector<EffectAnnotation>::iterator end() const { return impl->annotations.end(); }

		inline EffectAnnotationCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectAnnotationCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	private:
		XNPP_API EffectAnnotationCollection(size_t count);

		struct Implementation {
			std::vector<EffectAnnotation> annotations;
		};

		std::shared_ptr<Implementation> impl;

		friend class Effect;
		friend class EffectTechnique;
	};
}

#endif