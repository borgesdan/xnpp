#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECT_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECT_HPP

#include "../GraphicsResource.hpp"
#include <memory>
#include <vector>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class EffectTechnique;
	class EffectTechniqueCollection;
	class EffectParameterCollection;

	//Used to set and query effects, and to choose techniques.
	class Effect : public GraphicsResource {
	public:
		~Effect() = default;

		XNPP_API Effect(GraphicsDevice const& graphicsDevice, std::vector<uint8_t> const& effectCode);

		inline EffectTechnique CurrentTechnique() const;
		inline void CurrentTechnique(EffectTechnique const& value);

		inline EffectTechniqueCollection Techniques() const;
		inline EffectParameterCollection Parameters() const;

		inline Effect(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(Effect const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:		
		virtual void OnApply(){}

	private:
		void OnLostDevice(){}
		void OnResetDevice(){}

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;	
	};
}

#endif