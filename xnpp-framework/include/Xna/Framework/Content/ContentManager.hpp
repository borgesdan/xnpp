#pragma once

#include "Xna/Framework/Color.hpp"
#include "Xna/Framework/Game/GameServiceContainer.hpp"
#include "Xna/Framework/MathTypes.hpp"
#include "Xna/Internal/Misc.hpp"
#include "Xna/Macros/Api.hpp"
#include <any>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <Xna/CSharp/IO/BinaryReader.hpp>
#include <Xna/CSharp/IO/FileStream.hpp>
#include <Xna/CSharp/Attribute.hpp>
#include <Xna/CSharp/Type.hpp>

namespace Xna {
	class ContentReader;
	class ContentTypeReader;
	class ContentTypeReaderManager;
	class ContentReader;	

	//////////////////////////////////////////////////////////////////
	// CONTENT MANAGER												//
	//////////////////////////////////////////////////////////////////

	//The ContentManager is the run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager {
	public:		
		//Initializes a new instance of ContentManager.
		XNPP_API ContentManager(GameServiceContainer* serviceProvider, std::filesystem::path const& rootDirectory = "");

		XNPP_API GameServiceContainer* ServiceProvider() const;

		//Gets or sets the root directory associated with this ContentManager.
		XNPP_API std::filesystem::path RootDirectory() const;
		//Gets or sets the root directory associated with this ContentManager.
		XNPP_API void RootDirectory(std::filesystem::path const& value);

		//Disposes all data that was loaded by this ContentManager.
		virtual void Unload() {}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T> T Load(std::filesystem::path const& assetName) {
			if (assetName.empty())
				throw CSharp::InvalidOperationException("assetName is empty.");

			auto obj3 = ReadAsset<T>(assetName);

			return obj3;
		}

	protected:
		template <typename T> T ReadAsset(std::filesystem::path const& assetName);

		XNPP_API std::unique_ptr<CSharp::Stream> OpenStream(std::filesystem::path const& assetName);

	public:
		std::filesystem::path rootDirectory;
		std::filesystem::path fullRootDirectory;
		bool isRootDirectoryAbsolute{ false };
		GameServiceContainer* serviceProvider;
		std::shared_ptr<std::vector<uint8_t>> byteBuffer;
	};

	//////////////////////////////////////////////////////////////////
	// ATTRIBUTE													//
	//////////////////////////////////////////////////////////////////

	struct ContentSerializerAttribute : public CSharp::Attribute {
		~ContentSerializerAttribute() = default;

		std::string ElementName;
		bool FlattenContent{ false };
		bool Optional{ false };
		bool AllowNull{ true };
		bool SharedResource{ false };
		//string.IsNullOrEmpty(this.collectionItemName) ? "Item"
		std::string CollectionItemName;

		bool HasCollectionItemName() const noexcept {
			return !CollectionItemName.empty();
		}
	};

	//////////////////////////////////////////////////////////////////
	// CONTENT READER												//
	//////////////////////////////////////////////////////////////////	

	//A worker object that implements most of ContentManager.Load. A new ContentReader is constructed for each asset loaded.
	class ContentReader : public CSharp::BinaryReader {
	public:
		//Gets the ContentManager associated with the ContentReader.
		inline Xna::ContentManager* ContentManager() const;

		//Gets the name of the asset currently being read by this ContentReader.
		inline std::filesystem::path AssetName() const;

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		inline T ReadObject() {
			return ReadObjectInternal<T>(nullptr);
		}

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		T ReadObject(T const& existingInstance) {
			return ReadObjectInternal<T>(existingInstance);
		}

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		T ReadObject(ContentTypeReader& reader) {
			return ReadObjectInternal<T>(reader, nullptr);
		}

		//Overloaded. Reads a single managed object from the current stream. Can be called recursively.
		template <typename T>
		T ReadObject(ContentTypeReader& reader, T const& existingInstance) {
			return ReadObjectInternal<T>(reader, existingInstance);
		}

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T> T ReadRawObject();

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T> T ReadRawObject(T const& existingInstance);

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		T ReadRawObject(ContentTypeReader& typeReader) {
			return InvokeReader<T>(typeReader, nullptr);
		}

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		T ReadRawObject(ContentTypeReader& typeReader, T const& existingInstance) {
			return InvokeReader<T>(typeReader, existingInstance);
		}

		//Reads a link to an external file.
		template <typename T> T ReadExternalReference();

		//Reads a Vector2 value from the current stream.
		XNPP_API Vector2 ReadVector2();
		//Reads a Vector3 value from the current stream.
		XNPP_API Vector3 ReadVector3();
		// 	Reads a Vector4 value from the current stream.
		XNPP_API Vector4 ReadVector4();
		//Reads a Matrix value from the currently open stream.
		XNPP_API Matrix ReadMatrix();
		//Reads a Quaternion value from the current stream.
		XNPP_API Quaternion ReadQuaternion();
		//Reads a Color value from the currently open stream.
		XNPP_API Color ReadColor();

		//Reads a float value from the currently open stream.
		float ReadSingle() {
			const auto int32 = ReadUInt32();
			return *(float*)&int32;
		}

		//Reads a float value from the currently open stream.
		inline double ReadDouble() {
			const auto int64 = ReadUInt64();
			return *(double*)&int64;
		}

		XNPP_API std::shared_ptr<std::vector<uint8_t>> ReadByteBuffer(size_t size);		

		//private:

		XNPP_API static ContentReader Create(Xna::ContentManager* contentManager, std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName);
		
		template <typename T> T ReadAsset() {
			const auto sharedResourceCount = ReadHeader();
			const auto obj = ReadObject<T>();
			return obj;
		}
	private:
		XNPP_API ContentReader(Xna::ContentManager* contentManager, std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName, int32_t graphicsProfile);

		XNPP_API static void PrepareStream(std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName, int32_t& graphicsProfile);

		XNPP_API int32_t ReadHeader();

		template <typename T> T ReadObjectInternal(T* existingInstance);

		template <typename T>
		T ReadObjectInternal(ContentTypeReader& typeReader, T* existingInstance);

		template <typename T>
		T InvokeReader(ContentTypeReader& reader, T* existingInstance);

	public:
		XNPP_API std::filesystem::path GetPathToReference(std::filesystem::path const& referenceName);
		XNPP_API std::filesystem::path GetAbsolutePathToReference(std::filesystem::path const& referenceName);

		constexpr int32_t GraphicsProfile() const noexcept { return graphicsProfile; }

	public:
		static constexpr char PlatformLabel = 'w';
		static constexpr uint16_t XnbVersion = 5;
		static constexpr uint16_t XnbCompressedVersion = 32773;
		static constexpr uint16_t XnbVersionProfileMask = 32512;
		static constexpr int32_t XnbVersionProfileShift = 8;
		static constexpr int32_t XnbPrologueSize = 10;
		static constexpr int32_t XnbCompressedPrologueSize = 14;

		private:
		Xna::ContentManager* contentManager;
		std::filesystem::path assetName;
		std::vector<std::shared_ptr<ContentTypeReader>> typeReaders;
		int32_t graphicsProfile{ 0 };
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

	private:
		const CSharp::Type targetType;

		friend class ContentReader;
		friend class ContentTypeReaderManager;
	};

	template <typename T>
	class ContentTypeReaderT : public ContentTypeReader {
	protected:
		ContentTypeReaderT() : ContentTypeReader(CSharp::Typeof<T>()) {}

		virtual std::any Read(ContentReader& input, std::any& existingInstance) override {
			if (!existingInstance.has_value()) {
				//TODO: removido Xna::Misc::ReturnDefaultOrNull<T>();
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

	//A manager that constructs and keeps track of type reader objects.
	class ContentTypeReaderManager {
	public:
		//Looks up a reader for the specified type.
		XNPP_API std::shared_ptr<ContentTypeReader> GetTypeReader(CSharp::Type const& targetType);		

	private:
		static std::vector<std::shared_ptr<ContentTypeReader>> ReadTypeManifest(size_t typeCount, ContentReader& contentReader);

		ContentTypeReaderManager(ContentReader const& contentReader);

		static std::shared_ptr<ContentTypeReader> GetTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::vector<std::shared_ptr<ContentTypeReader>>& newTypeReaders);

		static bool InstantiateTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::shared_ptr<ContentTypeReader>& reader);

		static void AddTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::shared_ptr<ContentTypeReader> const& reader);

		static void RoolbackAddReaders(std::vector<ContentTypeReader> const& newTypeReaders);

		template <typename T>
		static void RollbackAddReader(std::map<T, std::shared_ptr<ContentTypeReader>>& dictionary, ContentTypeReader const& reader);

		static std::shared_ptr<ContentTypeReader> GetTypeReader(CSharp::Type const& targetType, ContentReader const& contentReader);

		static bool ContainsTypeReader(CSharp::Type const& targetType);

	public:
		std::optional<ContentReader> contentReader;

		static inline auto nameToReader = std::map<std::string, std::shared_ptr<ContentTypeReader>>();
		static inline auto targetTypeToReader = std::map<CSharp::Type, std::shared_ptr<ContentTypeReader>>();
		static inline auto readerTypeToReader = std::map<CSharp::Type, std::shared_ptr<ContentTypeReader>>();

		friend class ContentReader;
	};

	//////////////////////////////////////////////////////////////////
	// IMPLS														//
	//////////////////////////////////////////////////////////////////

	template <typename T> T ContentManager::ReadAsset(std::filesystem::path const& assetName) {
		std::shared_ptr<CSharp::Stream> input = OpenStream(assetName);

		auto contentReader = ContentReader::Create(this, input, assetName);
		return contentReader.ReadAsset<T>();
	}

	template <typename T> T ContentReader::ReadExternalReference() {
		const auto referenceName = ReadString();
		return referenceName.empty() ? T() : contentManager->Load<T>(referenceName);
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
		return InvokeReader<T>(ContentTypeReaderManager::GetTypeReader(typeid(T), *this), nullptr);
	}

	inline Xna::ContentManager* ContentReader::ContentManager() const {
		return contentManager;
	}

	inline std::filesystem::path ContentReader::AssetName() const {
		return assetName.string();
	}

	template <typename T>
	void ContentTypeReaderManager::RollbackAddReader(std::map<T, std::shared_ptr<ContentTypeReader>>& dictionary, ContentTypeReader const& reader) {

		for (auto it = dictionary.begin(); it != dictionary.end();) {
			if (it->second.get() == &reader)
				dictionary.erase(it);
			else
				++it;
		}
	}
}