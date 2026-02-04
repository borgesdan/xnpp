#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"
#include <stdexcept>

namespace Xna {
	ContentWriter::ContentWriter(
		ContentCompiler compiler,
		std::shared_ptr<CSharp::Stream> output,
		Xna::TargetPlatform targetPlatform,
		GraphicsProfile targetProfile,
		bool compressContent,
		std::filesystem::path rootDirectory,
		std::filesystem::path referenceRelocationPath) : BinaryWriter() {

		impl = std::make_shared<Implementation>();
		impl->compiler = compiler;
		impl->finalOutput = output;
		impl->targetPlatform = targetPlatform;
		impl->targetProfile = targetProfile;
		impl->compressContent = compressContent;
		impl->rootDirectory = rootDirectory;
		impl->referenceRelocationPath = referenceRelocationPath;
		impl->headerData = std::make_shared<CSharp::MemoryStream>();
		impl->contentData = std::make_shared<CSharp::MemoryStream>();
		OutStream(impl->contentData);
	}

	std::shared_ptr<ContentTypeWriter> ContentWriter::GetTypeWriter(CSharp::Type const& type, int32_t& typeIndex) {
		if (impl->typeTable.contains(type))
			return impl->typeWriters[static_cast<size_t>(impl->typeTable[type])];

		//TODO: [!] não implementado
		std::vector<CSharp::Type> dependencies;
		const auto typeWriter = impl->compiler.GetTypeWriter(type, dependencies);

		if (typeWriter == nullptr)
			return nullptr;

		typeIndex = impl->typeWriters.size();	
		impl->typeWriters.push_back(typeWriter);
		impl->typeTable[type] = typeIndex;

		return typeWriter;
	}

	void ContentWriter::WriteSharedResources() {
		while (impl->sharedResources.size() > 0) {
			auto& obj = impl->sharedResources.front();
			WriteObject(&obj);
			impl->sharedResources.pop();
		}
	}

	void ContentWriter::FlushOutput() {
		WriteSharedResources();
		WriteHeader();
		WriteFinalOutput();
	}

	void ContentWriter::WriteHeader()
	{
		OutStream(impl->headerData);
		Write7BitEncodedInt(static_cast<int32_t>(impl->typeWriters.size()));

		for(auto& typeWriter : impl->typeWriters) {
			BinaryWriter::Write(typeWriter->GetRuntimeReader(impl->targetPlatform));
			BinaryWriter::Write(typeWriter->TypeVersion());
		}
		
		Write7BitEncodedInt(static_cast<int32_t>(impl->sharedResourceNames.size()));
	}

	void ContentWriter::WriteFinalOutput() {
		OutStream(impl->finalOutput);
		BinaryWriter::Write(static_cast<uint8_t>(88)); //'X'
		BinaryWriter::Write(static_cast<uint8_t>(78)); //'N'
		BinaryWriter::Write(static_cast<uint8_t>(66)); //'B'

		if (impl->targetPlatform == TargetPlatform::Windows)
			BinaryWriter::Write(static_cast<uint8_t>(119)); //'w'

		else if (impl->targetPlatform == TargetPlatform::Xbox360) {
			BinaryWriter::Write(static_cast<uint8_t>(120)); //'x'
		}
		else
		{
			if (impl->targetPlatform != TargetPlatform::WindowsPhone)
				throw CSharp::NotSupportedException();

			BinaryWriter::Write(static_cast<uint8_t>(109));
		}
		if (impl->compressContent)
			WriteCompressedOutput();
		else
			WriteUncompressedOutput();
	}

	void ContentWriter::WriteUncompressedOutput()
	{
		WriteVersionNumber(XnbVersion);
		const auto length1 = static_cast<int32_t>(impl->headerData->Length());
		const auto length2 = static_cast<int32_t>(impl->contentData->Length());
		BinaryWriter::Write(XnbPrologueSize + length1 + length2);

		const auto& headerDataBuffer = impl->headerData->GetBuffer();
		const auto& contentDataBuffer = impl->contentData->GetBuffer();

		OutStream()->Write(headerDataBuffer.data(), headerDataBuffer.size(), 0, length1);
		OutStream()->Write(contentDataBuffer.data(), contentDataBuffer.size(), 0, length2);
	}

	void ContentWriter::WriteCompressedOutput() {
		//TODO: [!] not implemented
		throw std::runtime_error("ContentWriter::WriteCompressedOutput is not implemented.");
	}

	void ContentWriter::WriteVersionNumber(uint16_t version) {
		version |= static_cast<uint16_t>(static_cast<int32_t>(impl->targetProfile) << XnbVersionProfileShift & XnbVersionProfileMask);
		BinaryWriter::Write(version);
	}

	void ContentWriter::Write(Vector2 const& value) {
		BinaryWriter::Write(value.X);
		BinaryWriter::Write(value.Y);
	}

	void ContentWriter::Write(Vector3 const& value) {
		BinaryWriter::Write(value.X);
		BinaryWriter::Write(value.Y);
		BinaryWriter::Write(value.Z);
	}

	void ContentWriter::Write(Vector4 const& value) {
		BinaryWriter::Write(value.X);
		BinaryWriter::Write(value.Y);
		BinaryWriter::Write(value.Z);
		BinaryWriter::Write(value.W);
	}

	void ContentWriter::Write(Matrix const& value) {
		BinaryWriter::Write(value.M11);
		BinaryWriter::Write(value.M12);
		BinaryWriter::Write(value.M13);
		BinaryWriter::Write(value.M14);
		BinaryWriter::Write(value.M21);
		BinaryWriter::Write(value.M22);
		BinaryWriter::Write(value.M23);
		BinaryWriter::Write(value.M24);
		BinaryWriter::Write(value.M31);
		BinaryWriter::Write(value.M32);
		BinaryWriter::Write(value.M33);
		BinaryWriter::Write(value.M34);
		BinaryWriter::Write(value.M41);
		BinaryWriter::Write(value.M42);
		BinaryWriter::Write(value.M43);
		BinaryWriter::Write(value.M44);
	}

	void ContentWriter::Write(Quaternion const& value) {
		BinaryWriter::Write(value.X);
		BinaryWriter::Write(value.Y);
		BinaryWriter::Write(value.Z);
		BinaryWriter::Write(value.W);
	}
}