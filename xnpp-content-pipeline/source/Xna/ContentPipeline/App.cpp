#include "Xna/ContentPipeline/App.hpp"
#include "Xna/ContentPipeline/Importers/TextureImporter.hpp"
#include "Xna/ContentPipeline/Importers/FontDescriptionImporter.hpp"
#include "Xna/ContentPipeline/Importers/WavImporter.hpp"
#include "Xna/ContentPipeline/Importers/WmaImporter.hpp"

#include "Xna/ContentPipeline/Processors/TextureProcessor.hpp"
#include "Xna/ContentPipeline/Processors/FontDescriptionProcessor.hpp"
#include "Xna/ContentPipeline/Processors/SoundEffectProcessor.hpp"
#include "Xna/ContentPipeline/Processors/SongProcessor.hpp"

#include "Xna/ContentPipeline/Serialization/Compiler/Writers/TextureWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/Texture2DWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/SpriteFontWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/ListWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/RectangleWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingBoxWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingFrustumWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingSphereWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/PrimitivesWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/Vector3Writer.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/SoundEffectWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/SongWriter.hpp"

namespace Xna {
	void App::RegisterImporters() {
		App::GetImporterRegistry().Add(Texture2DImporter(), { "Texture2DImporter" });
		App::GetImporterRegistry().Add(FontDescriptionImporter(), { "FontDescriptionImporter" });
		App::GetImporterRegistry().Add(WavImporter(), { "WavImporter" });
		App::GetImporterRegistry().Add(WmaImporter(), { "WmaImporter" });
	}
	void App::RegisterProcessors() {
		App::GetProcessorRegistry().Add(TextureProcessor(), { "TextureProcessor" });
		App::GetProcessorRegistry().Add(Texture2DProcessor(), { "Texture2DProcessor" });
		App::GetProcessorRegistry().Add(FontDescriptionProcessor(), { "FontDescriptionProcessor" });
		App::GetProcessorRegistry().Add(SoundEffectProcessor(), { "SoundEffectProcessor" });
		App::GetProcessorRegistry().Add(SongProcessor(), { "SongProcessor" });
	}
	void App::RegisterWriters() {
		App::GetWriterRegistry().Add(RectangleWriter(), { "RectangleWriter" });
		App::GetWriterRegistry().Add(BoundingBoxWriter(), { "BoundingBoxWriter" });
		App::GetWriterRegistry().Add(BoundingFrustumWriter(), { "BoundingFrustumWriter" });
		App::GetWriterRegistry().Add(BoundingSphereWriter(), { "BoundingSphereWriter" });
		App::GetWriterRegistry().Add(Vector3Writer(), { "Vector3Writer" });
		App::GetWriterRegistry().Add(BooleanWriter(), { "BooleanWriter" });
		App::GetWriterRegistry().Add(ByteWriter(), { "ByteWriter" });
		App::GetWriterRegistry().Add(CharWriter(), { "CharWriter" });
		App::GetWriterRegistry().Add(Int16Writer(), { "Int16Writer" });
		App::GetWriterRegistry().Add(Int32Writer(), { "Int32Writer" });
		App::GetWriterRegistry().Add(Int64Writer(), { "Int64Writer" });
		App::GetWriterRegistry().Add(UInt16Writer(), { "UInt16Writer" });
		App::GetWriterRegistry().Add(UInt32Writer(), { "UInt32Writer" });
		App::GetWriterRegistry().Add(UInt64Writer(), { "UInt64Writer" });

		App::GetWriterRegistry().Add(Texture2DWriter(), { "Texture2DWriter" });
		App::GetWriterRegistry().Add(SpriteFontWriter(), { "SpriteFontWriter" });
		App::GetWriterRegistry().Add(ListWriter<Rectangle>("Rectangle"), { "ListWriter<Rectangle>" });
		App::GetWriterRegistry().Add(ListWriter<uint32_t>("Uint32"), { "ListWriter<uint32_t>" });
		App::GetWriterRegistry().Add(ListWriter<Vector3>("Vector3"), { "ListWriter<Vector3>" });
		
		App::GetWriterRegistry().Add(SoundEffectWriter(), {"SoundEffectWriter"});
		App::GetWriterRegistry().Add(SongWriter(), {"SongWriter"});
	}

	void App::Register() {
		RegisterImporters();
		RegisterProcessors();
		RegisterWriters();	
	}

	TypeRegistry<IContentImporter>& App::GetImporterRegistry() {
		static TypeRegistry<IContentImporter> importerRegistrator;
		return importerRegistrator;
	}

	TypeRegistry<IContentProcessor>& App::GetProcessorRegistry() {
		static TypeRegistry<IContentProcessor> processorRegistrator;
		return processorRegistrator;
	}

	TypeRegistry<ContentTypeWriter>& App::GetWriterRegistry() {
		static TypeRegistry<ContentTypeWriter> writerRegistrator;
		return writerRegistrator;
	}
}