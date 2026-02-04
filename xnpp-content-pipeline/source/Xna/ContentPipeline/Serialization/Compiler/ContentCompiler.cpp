#include "Xna/ContentPipeline/Serialization/Compiler/ContentCompiler.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"
#include <stdexcept>
#include <ranges>
#include "Xna/ContentPipeline/App.hpp"

namespace Xna {

	ContentCompiler::ContentCompiler() {		
		impl = std::make_shared<Implementation>();

		const auto& writers = App::GetWriterRegistry().GetRegistries();

		for (auto& writer : writers)
			AddTypeWriter(writer);

		for (auto& writer : impl->writerInstances | std::views::values)
			InitializeTypeWriter(writer);
	}

	void ContentCompiler::Compile(
		std::shared_ptr<CSharp::Stream> const& output,
		std::any const& value,
		TargetPlatform targetPlatform,
		GraphicsProfile targetProfile,
		bool compressContent,
		std::filesystem::path const& rootDirectory,
		std::filesystem::path const& referenceRelocationPath) {

		if (output == nullptr)
			throw std::invalid_argument("The argument 'output (Stream)' is null.");
		if(!value.has_value())
			throw std::invalid_argument("The argument 'output' is null.");
		if(rootDirectory.empty())
			throw std::invalid_argument("The argument 'rootDirectory' is empty.");
		if(referenceRelocationPath.empty())
			throw std::invalid_argument("The argument 'referenceRelocationPath' is empty.");

		if (compressContent)
			compressContent = ShouldCompressContent(targetPlatform, value);

		auto contentWriter = ContentWriter(*this, output, targetPlatform, targetProfile, compressContent, rootDirectory, referenceRelocationPath);		
		
		contentWriter.WriteObject<std::any>(&value); //TODO: [!] verificar
		contentWriter.FlushOutput();
	}

	void ContentCompiler::AddTypeWriter(std::shared_ptr<ContentTypeWriter> const& writer) {
		const auto targetType = writer->TargetType();

		if (impl->writerInstances.contains(targetType))
			throw std::runtime_error("The ContentCompiler already contains the specified instance of ContentWriter.");

		impl->writerInstances[targetType] = writer;
		impl->writerDependencies[writer] = std::vector<CSharp::Type>();
	}

	void ContentCompiler::InitializeTypeWriter(std::shared_ptr<ContentTypeWriter>& writer) {
		impl->initializeContext.push(writer);
		writer->DoInitialize(*this);
		impl->initializeContext.pop(); //TODO: [?] push e pop?
	}

	bool ContentCompiler::ShouldCompressContent(TargetPlatform targetPlatform, std::any const& value) {
		return targetPlatform != TargetPlatform::WindowsPhone &&
			GetTypeWriterInternal(value.type())->ShouldCompressContent(targetPlatform, value);
	}

	std::shared_ptr<ContentTypeWriter> ContentCompiler::GetTypeWriter(CSharp::Type type) {
		const auto typeWriter = GetTypeWriterInternal(type);

		if(typeWriter)
			RecordDependency(typeWriter->TargetType());

		return typeWriter;
	}

	std::shared_ptr<ContentTypeWriter> ContentCompiler::GetTypeWriter(CSharp::Type const& type, std::vector<CSharp::Type>& dependencies) {
		const auto typeWriterInternal = GetTypeWriterInternal(type);
		dependencies = impl->writerDependencies[typeWriterInternal];
		return typeWriterInternal;
	}

	std::shared_ptr<ContentTypeWriter> ContentCompiler::GetTypeWriterInternal(CSharp::Type const& type) {
		std::shared_ptr<ContentTypeWriter> writer;
		
		if (!impl->writerInstances.contains(type))
		{
			return nullptr;
		}
		else {
			writer = impl->writerInstances[type];
		}

		return writer;
	}

	void ContentCompiler::RecordDependency(CSharp::Type const& type) {
		auto& writerDependency = impl->writerDependencies[impl->initializeContext.top()];
		if (std::find(writerDependency.begin(), writerDependency.end(), type) != writerDependency.end())
			return;

		writerDependency.push_back(type);
	}
}