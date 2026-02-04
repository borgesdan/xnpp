#include "Xna/Framework/Graphics/RasterizerState.hpp"

namespace Xna {
	RasterizerState RasterizerState::CullNone()
	{
		auto raster = RasterizerState();
		raster.impl->cullMode = Xna::CullMode::None;
		return raster;
	}

	RasterizerState RasterizerState::CullClockwise()
	{
		auto raster = RasterizerState();
		raster.impl->cullMode = Xna::CullMode::CullClockwiseFace;
		return raster;
	}

	RasterizerState RasterizerState::CullCounterClockwise()
	{
		auto raster = RasterizerState();
		raster.impl->cullMode = Xna::CullMode::CullCounterClockwiseFace;
		return raster;
	}
}