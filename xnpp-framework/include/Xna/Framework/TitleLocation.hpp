#ifndef XNA_FRAMEWORK_TITLELOCATION_HPP
#define XNA_FRAMEWORK_TITLELOCATION_HPP

#include <filesystem>
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Internal class
	struct TitleLocation final {
	private:
		static inline const std::filesystem::path& Path() {
			if (_titleLocation.empty())
				Platform::System_GetExecutablePath(_titleLocation);

			return _titleLocation;
		}

		TitleLocation() = delete;
		TitleLocation(TitleLocation&) = delete;
		TitleLocation(TitleLocation&&) = delete;

	private:
		static inline std::filesystem::path _titleLocation = "";
		friend struct TitleContainer;
		friend struct ContentManager;
		friend struct ContentReader;
	};
}

#endif