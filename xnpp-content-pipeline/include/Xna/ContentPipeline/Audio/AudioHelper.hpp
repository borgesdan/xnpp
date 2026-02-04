#include <filesystem>
#include <memory>
#include <optional>
#include "AudioContent.hpp"
#include "Shared.hpp"

namespace Xna {
	struct AudioHelper final {
		AudioHelper();
		bool OpenFile(std::filesystem::path const& filename);
		bool IsSupportedAudioFile(AudioFileType& audioFileType);
		int64_t GetDuration();
		bool Convert(ConversionFormat conversionFormat, ConversionQuality quality, std::optional<std::filesystem::path> const& targetFileName, AudioContent& audioContent);

		inline AudioHelper(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(AudioHelper const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		std::vector<uint8_t> ConvertWmaToWavInMemory();

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}