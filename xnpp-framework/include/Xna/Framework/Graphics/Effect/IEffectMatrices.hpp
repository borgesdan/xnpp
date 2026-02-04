#ifndef XNA_FRAMEWORK_GRAPHICS_IEFFECTMATRICES_HPP
#define XNA_FRAMEWORK_GRAPHICS_IEFFECTMATRICES_HPP

#include "Xna/Framework/Matrix.hpp"

namespace Xna {
	struct IEffectMatrices {
		virtual ~IEffectMatrices() = default;
		virtual Matrix World() const = 0;
		virtual Matrix View() const = 0;
		virtual Matrix Projection() const = 0;
		virtual void World(Matrix const& value) = 0;
		virtual void View(Matrix const& value) = 0;
		virtual void Projection(Matrix const& value) = 0;
	};
}

#endif