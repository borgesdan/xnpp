#include "Xna/Framework/Graphics/DisplayModeCollection.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <cassert>

using Xna::CSharp::InvalidOperationException;

namespace Xna {
	DisplayMode& DisplayModeCollection::operator[](size_t index) {
		assert(index < displayModes.size() && "index >= collection size");
		return displayModes[index];
	}

	std::vector<DisplayMode> DisplayModeCollection::operator[](SurfaceFormat surfaceFormat) {
		std::vector<DisplayMode> displayModeList;
		size_t count{ 0 };

		for (size_t i = 0; i < displayModes.size(); ++i) {
			if (displayModes[i].Format() == surfaceFormat) {
				displayModeList.push_back(displayModes[i]);
				count++;
			}
		}		
		
		displayModeList.resize(count);
		return displayModeList;
	}
}