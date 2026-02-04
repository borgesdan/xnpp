#ifndef XNA_FRAMEWORK_CONTENT_CONTENTREADER_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTREADER_HPP

#include "_ContentManager.hpp"
#include "ContentTypeReader.hpp"
#include "Xna/CSharp/IO/BinaryReader.hpp"
#include <memory>
#include <vector>
#include <string>
#include <any>
#include <filesystem>
#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Vector4.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Quaternion.hpp"
#include "Xna/Framework/Color.hpp"
#include "_ContentTypeReaderManager.hpp"

namespace Xna {
	class ContentTypeReader;

	//A worker object that implements most of ContentManager.Load. A new ContentReader is constructed for each asset loaded.
	class ContentReader : public CSharp::BinaryReader {
	public:
		//Gets the ContentManager associated with the ContentReader.
		Xna::ContentManager ContentManager() const;

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
		template <typename T>
		T ReadRawObject() {
			return InvokeReader<T>(ContentTypeReaderManager::GetTypeReader(typeid(T), *this), nullptr);
		}

		//Overloaded. Reads a single managed object from the current stream as an instance of the specified type. 
		//If a base class of the actual object type is specified only data from the base type will be read.
		template <typename T>
		T ReadRawObject(T const& existingInstance) {
			return InvokeReader<T>(ContentTypeReaderManager::GetTypeReader(typeid(T), *this), existingInstance);
		}

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
		Vector2 ReadVector2();
		//Reads a Vector3 value from the current stream.
		Vector3 ReadVector3();
		// 	Reads a Vector4 value from the current stream.
		Vector4 ReadVector4();
		//Reads a Matrix value from the currently open stream.
		Matrix ReadMatrix();
		//Reads a Quaternion value from the current stream.
		Quaternion ReadQuaternion();
		//Reads a Color value from the currently open stream.
		Color ReadColor();

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

		std::shared_ptr<std::vector<uint8_t>> ReadByteBuffer(size_t size);

		inline ContentReader(std::nullptr_t) : CSharp::BinaryReader(nullptr) { impl = nullptr; }
		inline bool operator==(ContentReader const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		static ContentReader Create(Xna::ContentManager& contentManager, std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName);

		ContentReader(Xna::ContentManager& contentManager, std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName, int32_t graphicsProfile);

		static void PrepareStream(std::shared_ptr<CSharp::Stream> const& input, std::filesystem::path const& assetName, int32_t& graphicsProfile);

		template <typename T> T ReadAsset() {
			const auto sharedResourceCount = ReadHeader();
			const auto obj = ReadObject<T>();
			return obj;
		}

		int32_t ReadHeader();

		template <typename T> T ReadObjectInternal(T* existingInstance);

		template <typename T>
		T ReadObjectInternal(ContentTypeReader& typeReader, T* existingInstance) {
			return typeReader.TargetIsValueType
				? InvokeReader<T>(typeReader, existingInstance)
				: ReadObjectInternal<T>(existingInstance);
		}

		template <typename T>
		T InvokeReader(ContentTypeReader& reader, T* existingInstance) {
			auto contentTypeReader = dynamic_cast<ContentTypeReaderT<T>*>(&reader);

			if (contentTypeReader != nullptr) {
				T existingInstance1 = existingInstance == nullptr ? T() : *existingInstance;
				return contentTypeReader->Read(*this, existingInstance1);
			}
			else {
				auto anyInstance = std::any(existingInstance);
				auto obj = reader.Read(*this, anyInstance);

				return std::any_cast<T>(obj);;
			}
		}

	public:
		std::filesystem::path GetPathToReference(std::filesystem::path const& referenceName);
		std::filesystem::path GetAbsolutePathToReference(std::filesystem::path const& referenceName);

	public:
		struct Implementation;
		std::shared_ptr<Implementation> impl;

		friend class ContentManager;
	};

	struct ContentReader::Implementation {
		static constexpr char PlatformLabel = 'w';
		static constexpr uint16_t XnbVersion = 5;
		static constexpr uint16_t XnbCompressedVersion = 32773;
		static constexpr uint16_t XnbVersionProfileMask = 32512;
		static constexpr int32_t XnbVersionProfileShift = 8;
		static constexpr int32_t XnbPrologueSize = 10;
		static constexpr int32_t XnbCompressedPrologueSize = 14;

		std::optional<Xna::ContentManager> contentManager;
		std::filesystem::path assetName;
		std::vector<std::shared_ptr<ContentTypeReader>> typeReaders;
		int32_t graphicsProfile{ 0 };
	};

	template <typename T>
	T ContentReader::ReadExternalReference() {
		const auto referenceName = ReadString();
		return referenceName.empty() ? T() : impl->contentManager->Load<T>(referenceName);
	}

	template <typename T>
	T ContentReader::ReadObjectInternal(T* existingInstance) {
		auto num = Read7BitEncodedInt();

		if (num == 0)
			return T();

		auto index = num - 1;
		const auto readersSize = impl->typeReaders.size();

		if (index >= readersSize)
			throw CSharp::InvalidOperationException("Bad Xnb");

		auto reader = impl->typeReaders.at(index);
		return InvokeReader<T>(*reader, existingInstance);
	}

	inline Xna::ContentManager ContentReader::ContentManager() const {
		return impl->contentManager.value();
	}

	inline std::filesystem::path ContentReader::AssetName() const {
		return impl->assetName.string();
	}
}

#endif