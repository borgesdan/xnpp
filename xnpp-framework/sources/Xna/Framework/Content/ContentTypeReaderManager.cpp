#include "Xna/Framework/Content/ContentManager.hpp"
#include "Xna/Internal/App.hpp"
#include <algorithm>

namespace Xna {

	

	struct ContentTypeReaderParser {
		static inline CSharp::Type* Parse(std::string_view csharpTypeName) {
			auto sub = csharpTypeName.substr(0, csharpTypeName.find(","));

			if (CSharp::Type::NamedTypes().contains(sub.data())) {
				return &CSharp::Type::NamedTypes()[sub.data()];
			}

			return nullptr;
		}
	};

	std::vector<ContentTypeReader*> ContentTypeReaderManager::ReadTypeManifest(size_t typeCount, ContentReader* contentReader)
	{
		auto contentTypeReaderArray = std::vector<ContentTypeReader* >(typeCount);
		auto newTypeReader = std::vector<ContentTypeReader*>();

		for (size_t index = 0; index < typeCount; ++index) {
			const auto name = contentReader->ReadString();
			auto typeReader = GetTypeReader(name, contentReader, newTypeReader);

			const auto version = contentReader->ReadInt32();

			XNPP_CORE_ASSERT(version == typeReader->TypeVersion(), "Bad xnb type version.");

			contentTypeReaderArray[index] = typeReader;
		}

		if (!newTypeReader.empty()) {
			auto manager = ContentTypeReaderManager(contentReader);

			for (auto& contentTypeReader : newTypeReader)
				contentTypeReader->Initialize(manager);
		}

		return contentTypeReaderArray;
	}

	ContentTypeReader* ContentTypeReaderManager::GetTypeReader(
		std::string_view readerTypeName, ContentReader* contentReader,
		std::vector<ContentTypeReader*>& newTypeReaders)
	{
		auto cache = FindCache([readerTypeName](CacheData& d) {
			return d.name == readerTypeName;
			});

		if (cache)
			return cache->value;

		ContentTypeReader* reader = nullptr;

		if (!InstantiateTypeReader(readerTypeName, contentReader, &reader))
			return reader;

		AddTypeReader(readerTypeName, contentReader, reader);

		newTypeReaders.push_back(reader);

		return reader;
	}

	bool ContentTypeReaderManager::InstantiateTypeReader(
		std::string_view readerTypeName, ContentReader* contentReader,
		ContentTypeReader** reader) {

		auto type = ContentTypeReaderParser::Parse(readerTypeName);

		XNPP_CORE_ASSERT(type, "TypeReader type not founded.");		

		auto cache = FindCache([&](CacheData& d) {
			return d.readerType == *type;
			});		

		if (cache)
		{
			cache->name = readerTypeName;
			return false;
		}

		//TODO: corrigir isso
		*reader = App::GetContentTypeReaderActivator().CreateInstance(*type);

		XNPP_CORE_ASSERT(reader, "ContentTypeReaderActivator::CreateInstance return a null ContentReader. Type: {}", type->info().name());

		return true;
	}

	void ContentTypeReaderManager::AddTypeReader(
		std::string_view readerTypeName,
		ContentReader* contentReader,
		ContentTypeReader* reader) {

		const auto targetType = reader->TargetType();

		if (targetType != CSharp::Type::Empty()) {
			auto cache = FindCache([&](const CacheData& s) {
				return s.targetType == targetType;
				});

			//Registro duplicado
			if (cache) return;
		}

		CacheData cache;
		cache.name = readerTypeName;
		cache.targetType = targetType;
		cache.readerType = reader->GetType();		
		cache.value = reader;

		typeReadersCache.push_back(std::move(cache));
	}	

	ContentTypeReader* ContentTypeReaderManager::GetTypeReader(CSharp::Type const& targetType, ContentReader* contentReader) {

		auto cache = FindCache([&](CacheData& d) {
			return d.targetType == targetType;
			});		

		if (!cache) {
			auto& activator = App::GetContentTypeReaderActivator();
			auto& registries = activator.GetRegistries();

			for (const auto& reg : registries) {
				if (reg->TargetType() == targetType) {
					return reg.get();
				}
			}

			XNPP_CORE_ASSERT(false, "TypeReader not registered");
		}

		return cache->value;
	}

	bool ContentTypeReaderManager::ContainsTypeReader(CSharp::Type const& targetType) {
		auto cache = FindCache([&](CacheData& d) {
			return d.targetType == targetType;
			});

		return cache != nullptr;
	}
}