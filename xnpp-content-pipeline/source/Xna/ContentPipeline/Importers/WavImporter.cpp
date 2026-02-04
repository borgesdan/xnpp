#include "Xna/ContentPipeline/Importers/WavImporter.hpp"
#include <Xna/Framework/Audio/WavFile.hpp>
#include <Xna/CSharp/IO/FileStream.hpp>
#include <stdexcept>

namespace Xna {
	AudioContent WavImporter::ImportT(std::filesystem::path const& filename, ContentImporterContext& context) {
		try {
			auto stream = std::make_shared<CSharp::FileStream>(filename, CSharp::FileMode::Open);
			auto wavFile = WavFile::Open(stream);
			AudioContent audioContent;
			audioContent.Format = InternalAudioFormat(*wavFile.Format());
			audioContent.Duration = wavFile.Duration();
			audioContent.LoopLength = wavFile.LoopLength();
			audioContent.LoopStart = wavFile.LoopStart();
			audioContent.Data = *wavFile.Data();
			audioContent.FileType = AudioFileType::Wav;
			audioContent.ContentItem.Identity.SourceFileName = filename;

			return audioContent;
		}
		catch (std::exception const& ex) {
			std::string error("Error reading WAV file. Ex:");
			error.append(ex.what());
			throw std::runtime_error(error);
		}
	}
}