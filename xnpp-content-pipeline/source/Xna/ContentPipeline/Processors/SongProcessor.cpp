#include "Xna/ContentPipeline/Processors/SongProcessor.hpp"
#include <stdexcept>

namespace Xna {
	SongContent SongProcessor::ProcessT(AudioContent& input, ContentProcessorContext& context) {
		if (input.FileType && input.FileType != AudioFileType::Mp3 && input.FileType != AudioFileType::Wav && input.FileType != AudioFileType::Wma)
			throw std::runtime_error("Unsupported format. Song processor supports MP3, WMA and WAV files.");

		auto str = context.OutputFilename();
		str.replace_extension(input.ContentItem.Identity.SourceFileName.extension());

		auto directoryName = str.parent_path();

		try {
			if (!std::filesystem::exists(directoryName))
				std::filesystem::create_directory(directoryName);

			std::filesystem::copy_file(input.ContentItem.Identity.SourceFileName, str, std::filesystem::copy_options::overwrite_existing);
		}
		catch (std::exception const& ex) {
			std::string error("The directory creation or file copying action failed. Ex: ");
			error.append(ex.what());

			throw std::runtime_error(error);
		}	

		context.AddOutputFile(str);
		auto content = SongContent(str.filename(), static_cast<int32_t>(input.Duration.TotalMilliseconds()));
		return content;
	}
}