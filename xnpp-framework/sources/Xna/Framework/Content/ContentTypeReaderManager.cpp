#include "Xna/Framework/Content/ContentTypeReaderManager.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Framework/Content/ContentTypeActivator.hpp"
#include "Xna/Framework/Content/ContentTypeReader.hpp"
#include "Xna/Internal/App.hpp"

namespace Xna {
	ContentTypeReaderManager::ContentTypeReaderManager(ContentReader const& contentReader) {
		internalContentTypeReaderManager = std::make_shared<ContentTypeReaderManager::Implementation>();
		internalContentTypeReaderManager->contentReader = contentReader;
	}

	std::vector<std::shared_ptr<ContentTypeReader>> ContentTypeReaderManager::ReadTypeManifest(size_t typeCount, ContentReader& contentReader) {
		auto contentTypeReaderArray = std::vector<std::shared_ptr<ContentTypeReader>>(typeCount);

		std::vector<std::shared_ptr<ContentTypeReader>> newTypeReader;

		for (size_t index = 0; index < typeCount; ++index) {
			auto typeReader = GetTypeReader(contentReader.ReadString(), contentReader, newTypeReader);

			if (contentReader.ReadInt32() != typeReader->TypeVersion())
				throw CSharp::InvalidOperationException("Bad xnb type version.");

			contentTypeReaderArray[index] = typeReader;
		}

		if (!newTypeReader.empty()) {
			auto manager = ContentTypeReaderManager(contentReader);

			for (auto& contentTypeReader : newTypeReader)
				contentTypeReader->Initialize(manager);
		}

		return contentTypeReaderArray;
	}

	std::shared_ptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(CSharp::Type const& targetType) {
		return GetTypeReader(targetType, *internalContentTypeReaderManager->contentReader);
	}

	std::shared_ptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
		std::vector<std::shared_ptr<ContentTypeReader>>& newTypeReaders) {
		auto finded = Implementation::nameToReader.find(readerTypeName);

		if (finded != Implementation::nameToReader.end())
			return finded->second;

		std::shared_ptr<ContentTypeReader> reader;
		if (!InstantiateTypeReader(readerTypeName, contentReader, reader))
			return reader;			
		
		AddTypeReader(readerTypeName, contentReader, reader);

		newTypeReaders.push_back(reader);

		return reader;
	}

	bool ContentTypeReaderManager::InstantiateTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
		std::shared_ptr<ContentTypeReader>& reader) {

		//const auto type = CSharp::Type::GetType(readerTypeName);
		const auto type = ContentTypeReaderParser::Parse(readerTypeName);

		if (type == nullptr) {
			throw CSharp::InvalidOperationException("TypeReader type not founded.");
		}

		auto finded = Implementation::readerTypeToReader.find(*type);

		if (finded != Implementation::readerTypeToReader.end())
		{
			Implementation::nameToReader.emplace(readerTypeName, reader );
			return false;
		}
		
		reader = App::GetContentTypeReaderActivator().CreateInstance(*type);
		//reader = ContentTypeReaderActivator::CreateInstance(*type);

		if (reader == nullptr) {
			std::string error = "ContentTypeReaderActivator::CreateInstance return a null ContentReader. Type: ";
			error.append(type->info().name());
			throw CSharp::InvalidOperationException(error);
		}			

		return true;
	}

	void ContentTypeReaderManager::AddTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
		std::shared_ptr<ContentTypeReader> const& reader) {

		const auto targetType = reader->TargetType();

		if (targetType != CSharp::Type(typeid(nullptr))) {
			if (Implementation::targetTypeToReader.find(targetType) != Implementation::targetTypeToReader.end())
				return; //TODO: throw CSharp::InvalidOperationException("TypeReaderDuplicate");

			Implementation::targetTypeToReader.emplace(targetType, reader);
		}
		
		Implementation::readerTypeToReader.emplace(reader->GetType(), reader);
		Implementation::nameToReader.emplace(readerTypeName, reader);
	}

	void ContentTypeReaderManager::RoolbackAddReaders(std::vector<ContentTypeReader> const& newTypeReaders) {
		if (newTypeReaders.empty())
			return;

		for (auto& newTypeReader : newTypeReaders) {
			RollbackAddReader<std::string>(Implementation::nameToReader, newTypeReader);
			RollbackAddReader<CSharp::Type>(Implementation::targetTypeToReader, newTypeReader);
			RollbackAddReader<CSharp::Type>(Implementation::readerTypeToReader, newTypeReader);
		}
	}

	std::shared_ptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(CSharp::Type const& targetType, ContentReader const& contentReader) {
		if (targetType == CSharp::Type(typeid(nullptr)))
			throw CSharp::ArgumentException("targetType");

		auto find = Implementation::targetTypeToReader.find(targetType);

		if (find == Implementation::targetTypeToReader.end())
			throw CSharp::InvalidOperationException("TypeReaderNotRegistered");

		return find->second;
	}

	bool ContentTypeReaderManager::ContainsTypeReader(CSharp::Type const& targetType) {
		return Implementation::targetTypeToReader.contains(targetType);
	}
}