#pragma once

#include <any>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <Xna/CSharp/Attribute.hpp>
#include <Xna/CSharp/Exception.hpp>
#include <Xna/CSharp/IO.hpp>
#include <Xna/CSharp/Type.hpp>
#include <Xna/Framework/Color.hpp>
#include <Xna/Framework/Game/GameServiceContainer.hpp>
#include <Xna/Framework/MathTypes.hpp>
#include <Xna/Internal/Misc.hpp>
#include <Xna/Macros/Api.hpp>
#include <Xna/Macros/CoreAssert.hpp>

namespace Xna {
	class ContentReader;
	class ContentTypeReader;
	class ContentTypeReaderManager;
	class ContentReader;

	//////////////////////////////////////////////////////////////////
	// CONTENT MANAGER												//
	//////////////////////////////////////////////////////////////////

	//The ContentManager is the run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class XNPP_API ContentManager {
	public:
		//Initializes a new instance of ContentManager.
		ContentManager(GameServiceContainer* serviceProvider, std::filesystem::path const& rootDirectory = "");

		GameServiceContainer* ServiceProvider() const;

		//Gets or sets the root directory associated with this ContentManager.
		const std::filesystem::path& RootDirectory() const noexcept { return rootDirectory; }
		//Gets or sets the root directory associated with this ContentManager.
		void RootDirectory(std::filesystem::path const& value);

		//Disposes all data that was loaded by this ContentManager.
		virtual void Unload() {}

		//Loads an asset that has been processed by the Content Pipeline.
		//TODO: Adicionar o arquivo carregado a um cache
		//TODO: Adicionar um recarregamento de arquivo, após implementação de cache
		template <typename T> T Load(std::filesystem::path const& assetName, bool addCache = true, bool reloadFile = false) {
			XNPP_CORE_ASSERT(!assetName.empty(), "Asset name is empty.");

			auto obj3 = ReadAsset<T>(assetName);

			return obj3;
		}

		[[nodiscard]] ContentTypeReaderManager* GetTypeReaderManager() const noexcept { return readerManager.get(); }

	protected:
		template <typename T> T ReadAsset(std::filesystem::path const& assetName);

		XNPP_API std::unique_ptr<Stream> OpenStream(std::filesystem::path const& assetName);

	public:
		std::filesystem::path rootDirectory;
		std::filesystem::path fullRootDirectory;
		bool isRootDirectoryAbsolute{ false };
		GameServiceContainer* serviceProvider{ nullptr };
		std::vector<uint8_t> byteBuffer;
		std::unique_ptr<ContentTypeReaderManager> readerManager;
	};

	//////////////////////////////////////////////////////////////////
	// CONTENT READER												//
	//////////////////////////////////////////////////////////////////	

	//A worker object that implements most of ContentManager.Load. A new ContentReader is constructed for each asset loaded.
	class XNPP_API ContentReader : public BinaryReader {
	public:
		//Gets the ContentManager associated with the ContentReader.
		[[nodiscard]] Xna::ContentManager* ContentManager() const noexcept { return contentManager; }

		//Gets the name of the asset currently being read by this ContentReader.
		[[nodiscard]] const std::filesystem::path& AssetName() const noexcept { return assetName; }

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		[[nodiscard]] T ReadObject() { return ReadObjectInternal<T>(nullptr); }

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		[[nodiscard]] T ReadObject(T const& existingInstance) { return ReadObjectInternal<T>(existingInstance); }

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		[[nodiscard]] T ReadObject(ContentTypeReader& reader) { return ReadObjectInternal<T>(reader, nullptr); }

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		[[nodiscard]] T ReadObject(ContentTypeReader& reader, T const& existingInstance) { return ReadObjectInternal<T>(reader, existingInstance); }

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		[[nodiscard]] T ReadRawObject();

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		[[nodiscard]] T ReadRawObject(T const& existingInstance);

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		[[nodiscard]] T ReadRawObject(ContentTypeReader& typeReader) { return InvokeReader<T>(typeReader, nullptr); }

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		[[nodiscard]] T ReadRawObject(ContentTypeReader& typeReader, T const& existingInstance) { return InvokeReader<T>(typeReader, existingInstance); }

		//Reads a link to an external file.
		template <typename T>
		[[nodiscard]] T ReadExternalReference() {
			const auto referenceName = ReadString();
			return referenceName.empty() ? T() : contentManager->Load<T>(referenceName);
		}

		//Reads a Vector2 value from the current stream.
		[[nodiscard]] Vector2 ReadVector2();
		//Reads a Vector3 value from the current stream.
		[[nodiscard]] Vector3 ReadVector3();
		// 	Reads a Vector4 value from the current stream.
		[[nodiscard]] Vector4 ReadVector4();
		//Reads a Matrix value from the currently open stream.
		[[nodiscard]] Matrix ReadMatrix();
		//Reads a Quaternion value from the current stream.
		[[nodiscard]] Quaternion ReadQuaternion();

		//Reads a Color value from the currently open stream.
		[[nodiscard]] Color ReadColor() {
			const auto value = ReadUInt32();
			return Color(value);
		}

		//Reads a float value from the currently open stream.
		[[nodiscard]] float ReadSingle() {
			const auto int32 = ReadUInt32();
			return std::bit_cast<float>(int32);
		}

		//Reads a float value from the currently open stream.
		[[nodiscard]] double ReadDouble() {
			const auto int64 = ReadUInt64();
			return std::bit_cast<double>(int64);
		}

		const std::vector<uint8_t>& ReadByteBuffer(size_t size);

		//
		//Funções internas
		//

		[[nodiscard]] static ContentReader Create(Xna::ContentManager* contentManager, std::unique_ptr<Stream> input, std::filesystem::path const& assetName);

		template <typename T>
		[[nodiscard]] T ReadAsset() {
			const auto sharedResourceCount = ReadHeader();
			const auto obj = ReadObject<T>();
			return obj;
		}
	private:
		ContentReader(Xna::ContentManager* contentManager, std::unique_ptr<Stream> input, std::filesystem::path const& assetName);
		static void PrepareStream(Stream* input, std::filesystem::path const& assetName);
		[[nodiscard]] int32_t ReadHeader();

		template <typename T>
		[[nodiscard]] T ReadObjectInternal(T* existingInstance);

		template <typename T>
		[[nodiscard]] T ReadObjectInternal(ContentTypeReader& typeReader, T* existingInstance);

		template <typename T>
		[[nodiscard]] T InvokeReader(ContentTypeReader& reader, T* existingInstance);

	public:
		[[nodiscard]] std::filesystem::path GetPathToReference(std::filesystem::path const& referenceName);
		[[nodiscard]] std::filesystem::path GetAbsolutePathToReference(std::filesystem::path const& referenceName);


	private:
		Xna::ContentManager* contentManager;
		std::filesystem::path assetName;
		std::vector<ContentTypeReader*> typeReaders;
	};

	//////////////////////////////////////////////////////////////////
	// CONTENT TYPE READER											//
	//////////////////////////////////////////////////////////////////		

	//Worker for reading a specific managed type from a binary format.
	class ContentTypeReader {
	protected:
		//Creates a new instance of ContentTypeReader.
		ContentTypeReader(CSharp::Type const& type)
			: targetType(type), TargetIsValueType(type.IsValueType()) {}

		//Retrieves and caches nested type readers. Called by the framework at creation time.
		virtual void Initialize(ContentTypeReaderManager& manager) {}
		// Reads a strongly typed object from the current stream.
		virtual std::any Read(ContentReader& input, std::any& existingInstance) = 0;
		const bool TargetIsValueType{ true };

	public:
		//Gets the type handled by this reader component.
		CSharp::Type TargetType() const { return targetType; }
		// 	Gets a format version number for this type.
		virtual int32_t TypeVersion() const { return 0; }
		//Determines if deserialization into an existing object is possible.
		virtual bool CanDeserializeIntoExistingObject() const { return false; }

		virtual CSharp::Type GetType() const {
			return CSharp::Typeof<ContentTypeReader>();
		}

		virtual ~ContentTypeReader() = default;

	private:
		const CSharp::Type targetType;

		friend class ContentReader;
		friend class ContentTypeReaderManager;
	};

	template <typename T>
	class ContentTypeReaderT : public ContentTypeReader {
	protected:
		ContentTypeReaderT() : ContentTypeReader(CSharp::Typeof<T>()) {}

		~ContentTypeReaderT() override = default;

		virtual std::any Read(ContentReader& input, std::any& existingInstance) override {
			if (!existingInstance.has_value()) {
				
				return {};
			}
			else if (existingInstance.type() != typeid(T)) {
				throw CSharp::InvalidOperationException("BadXnbWrongType");
			}

			auto existingInstance1 = std::any_cast<T>(existingInstance);

			return Read(input, existingInstance1);
		}

		virtual T Read(ContentReader& input, T& existingInstance) = 0;

		virtual CSharp::Type GetType() const override {
			return CSharp::Typeof<ContentTypeReaderT<T>>();
		}

		friend class ContentReader;
		friend class ContentTypeReaderManager;
	};

	//////////////////////////////////////////////////////////////////
	// CONTENT TYPE READER	MANAGER									//
	//////////////////////////////////////////////////////////////////		

	struct CacheData {
		CSharp::Type targetType;
		CSharp::Type readerType;
		std::string name;
		ContentTypeReader* value; //Verificar uso de unique_ptr
	};

	//A manager that constructs and keeps track of type reader objects.
	class XNPP_API ContentTypeReaderManager {
	public:
		ContentTypeReaderManager() = default;

		void SetContentReader(ContentReader* reader) { _contentReader = reader; }

		//Looks up a reader for the specified type.
		ContentTypeReader* GetTypeReader(CSharp::Type const& targetType) {
			return GetTypeReader(targetType, _contentReader);
		}

		std::vector<ContentTypeReader*> ReadTypeManifest(size_t typeCount, ContentReader* contentReader);

	private:
		ContentTypeReaderManager(ContentReader* reader) : _contentReader(reader) {}

		ContentTypeReader* GetTypeReader(
			std::string_view readerTypeName,
			ContentReader* contentReader,
			std::vector<ContentTypeReader*>& newTypeReaders);

		ContentTypeReader* GetTypeReader(CSharp::Type const& targetType, ContentReader* contentReader);

		bool InstantiateTypeReader(std::string_view readerTypeName, ContentReader* contentReader,
			ContentTypeReader** reader);

		void AddTypeReader(std::string_view readerTypeName, ContentReader* contentReader,
			ContentTypeReader* reader);

		bool ContainsTypeReader(CSharp::Type const& targetType);

		template <typename Predicate>
		CacheData* FindCache(Predicate pred) {
			auto it = std::find_if(typeReadersCache.begin(), typeReadersCache.end(), pred);

			if (it != typeReadersCache.end())
				return &*it;

			return nullptr;
		}

	private:
		std::vector<CacheData> typeReadersCache = std::vector<CacheData>();
		ContentReader* _contentReader;
	};

	//////////////////////////////////////////////////////////////////
	// IMPLS														//
	//////////////////////////////////////////////////////////////////

	template <typename T> T ContentManager::ReadAsset(std::filesystem::path const& assetName) {
		auto input = OpenStream(assetName);

		auto contentReader = ContentReader::Create(this, std::move(input), assetName);
		return contentReader.ReadAsset<T>();
	}

	template <typename T>
	T ContentReader::ReadObjectInternal(T* existingInstance) {
		auto num = Read7BitEncodedInt();

		if (num == 0) {
			Xna::Misc::ReturnDefaultOrNull<T>();
		}

		auto index = num - 1;
		const auto readersSize = typeReaders.size();

		if (index >= readersSize)
			throw CSharp::InvalidOperationException("Bad Xnb");

		auto reader = typeReaders.at(index);
		return InvokeReader<T>(*reader, existingInstance);
	}

	template <typename T>
	T ContentReader::ReadObjectInternal(ContentTypeReader& typeReader, T* existingInstance) {
		return typeReader.TargetIsValueType
			? InvokeReader<T>(typeReader, existingInstance)
			: ReadObjectInternal<T>(existingInstance);
	}

	template <typename T>
	T ContentReader::InvokeReader(ContentTypeReader& reader, T* existingInstance) {
		auto contentTypeReader = dynamic_cast<ContentTypeReaderT<T>*>(&reader);

		if (contentTypeReader != nullptr) {
			T existingInstance1 = existingInstance == nullptr ? Xna::Misc::ReturnDefaultOrNull<T>() : *existingInstance;
			return contentTypeReader->Read(*this, existingInstance1);
		}
		else {
			auto anyInstance = std::any(existingInstance);
			auto obj = reader.Read(*this, anyInstance);

			return std::any_cast<T>(obj);;
		}
	}

	template <typename T>
	T ContentReader::ReadRawObject() {
		auto readerManager = contentManager->GetTypeReaderManager();
		auto typeReader = readerManager->GetTypeReader((typeid(T), *this));
		return InvokeReader<T>(typeReader, nullptr);
	}
}