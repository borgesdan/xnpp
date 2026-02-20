#include "Xna/ContentPipeline/Importers/OggImporter.hpp"
#include "Xna/ContentPipeline/Audio/AudioHelper.hpp"
#include <stdexcept>

namespace Xna {
	AudioContent OggImporter::ImportT(std::filesystem::path const& filename, ContentImporterContext& context) {
		auto audioHelper = AudioHelper();

		if (!audioHelper.OpenFile(filename))
			throw std::runtime_error("The file is not a supported audio file.");

		AudioFileType fileType;
		if (!audioHelper.IsSupportedAudioFile(fileType))
			throw std::runtime_error("The file is not a supported audio file.");

		if (fileType != AudioFileType::Ogg)
			throw std::runtime_error("The file is not a valid OGG file.");

		auto duration = audioHelper.GetDuration();

		if (duration < 0)
			throw std::runtime_error("The file's duration could not be obtained.");

		AudioContent content;
		content.ContentItem.Identity.SourceFileName = filename;
		content.Duration = CSharp::TimeSpan::FromMilliseconds(duration);
		content.FileType = fileType;

		return content;
	}
}