#include <filesystem>
#include <memory>
#include <optional>
#include "AudioContent.hpp"
#include "Shared.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct AudioHelper final {
		XNPP_API AudioHelper();
		XNPP_API bool OpenFile(std::filesystem::path const& filename);
		XNPP_API bool IsSupportedAudioFile(AudioFileType& audioFileType);
		XNPP_API int64_t GetDuration();
		XNPP_API bool Convert(ConversionFormat conversionFormat, ConversionQuality quality, std::optional<std::filesystem::path> const& targetFileName, AudioContent& audioContent);

		XNPP_DECLARE_NULL_IMPL_WRAPPER(AudioHelper, impl);

	private:
		XNPP_API std::vector<uint8_t> ConvertWmaToWavInMemory();

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}