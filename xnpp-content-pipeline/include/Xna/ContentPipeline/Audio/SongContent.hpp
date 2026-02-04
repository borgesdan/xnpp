#ifndef XNA_AUDIO_SONGCONTENT_HPP
#define XNA_AUDIO_SONGCONTENT_HPP

#include <filesystem>
#include <cstdint>

namespace Xna {
	struct SongContent {
		inline SongContent() = default;

		inline SongContent(std::filesystem::path const& filename, int32_t duration)
			: FileName(filename), Duration(duration) {}
		std::filesystem::path FileName{};
		int32_t Duration{ 0 };
	};
}

#endif