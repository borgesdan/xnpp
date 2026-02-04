#include "Xna/Framework/Graphics/DisplayModeCollection.hpp"
#include "Xna/CSharp/Exception.hpp"

using Xna::CSharp::InvalidOperationException;

namespace Xna {
	DisplayMode& DisplayModeCollection::operator[](size_t index) {
		if (index >= displayModes.size())
			throw InvalidOperationException("index >= collection size");

		return *displayModes[index];
	}

	std::vector<std::optional<DisplayMode>> DisplayModeCollection::operator[](SurfaceFormat surfaceFormat) {
		std::vector<std::optional<DisplayMode>> displayModeList;
		size_t count{ 0 };

		for (size_t i = 0; i < displayModes.size(); ++i) {
			if (displayModes[i]->Format() == surfaceFormat) {
				displayModeList.push_back(displayModes[i]);
				count++;
			}
		}		
		
		displayModeList.resize(count);
		return displayModeList;
	}
}