#include "Xna/Framework/Graphics/DepthStencilState.hpp"

namespace Xna {	

	DepthStencilState DepthStencilState::None() {
		auto stencil = DepthStencilState();
		stencil.impl->depthBufferEnable = false;
		stencil.impl->depthBufferWriteEnable = false;

		return stencil;
	}

	DepthStencilState DepthStencilState::Default() {
		auto stencil = DepthStencilState();
		stencil.impl->depthBufferEnable = true;
		stencil.impl->depthBufferWriteEnable = true;

		return stencil;
	}

	DepthStencilState DepthStencilState::DepthRead() {
		auto stencil = DepthStencilState();
		stencil.impl->depthBufferEnable = true;
		stencil.impl->depthBufferWriteEnable = false;

		return stencil;
	}	
}