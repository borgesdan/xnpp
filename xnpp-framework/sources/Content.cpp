#include "BasicReaders.hpp"
#include "Xna/Framework/Graphics/GraphicsContentHelper.hpp"
#include "pch.hpp"
#include "TitleLocation.hpp"
#include <Xna/CSharp/Path.hpp>
#include <Xna/Framework/Content/ContentManager.hpp>
#include <Xna/Framework/TitleContainer.hpp>
#include <Xna/Internal/App.hpp>
#include <Xna/Macros/CoreAssert.hpp>

//////////////////////////////////////////////////////////////////
// CONTENT MANAGER												//
//////////////////////////////////////////////////////////////////
namespace Xna {
	ContentManager::ContentManager(GameServiceContainer* serviceProvider, std::filesystem::path const& rootDirectory) {
		RootDirectory(rootDirectory);
		this->serviceProvider = serviceProvider;
		this->readerManager = std::make_unique<ContentTypeReaderManager>();
	}

	GameServiceContainer* ContentManager::ServiceProvider() const {
		return serviceProvider;
	}

	void ContentManager::RootDirectory(std::filesystem::path const& value) {
		rootDirectory = value;
		fullRootDirectory = value;
		isRootDirectoryAbsolute = TitleContainer::IsPathAbsolute(value);

		if (!isRootDirectoryAbsolute)
			return;

		fullRootDirectory = CSharp::Path::Combine(TitleLocation::Path(), value);
	}

	std::unique_ptr<Stream> ContentManager::OpenStream(std::filesystem::path const& assetName) {
		auto asset = assetName;
		asset.replace_extension(".xnb");
		auto path = CSharp::Path::Combine(fullRootDirectory, asset);

		auto cleanPath = TitleContainer::GetCleanPath(path);

		if (isRootDirectoryAbsolute)
		{
			auto stream = new FileStream(cleanPath, FileMode::Open);
			return std::unique_ptr<FileStream>(stream);
		}

		return TitleContainer::OpenStream(cleanPath);
	}
}

//////////////////////////////////////////////////////////////////
// CONTENT READER												//
//////////////////////////////////////////////////////////////////
namespace Xna {
	static constexpr uint8_t PlatformLabel = static_cast<uint8_t>('w');
	static constexpr uint16_t XnbVersion = 5;
	static constexpr uint16_t XnbCompressedVersion = 32773;
	static constexpr uint16_t XnbVersionProfileMask = 32512;
	static constexpr int32_t XnbVersionProfileShift = 8;
	static constexpr int32_t XnbPrologueSize = 10;
	static constexpr int32_t XnbCompressedPrologueSize = 14;

	ContentReader::ContentReader(Xna::ContentManager* contentManager, std::unique_ptr<Stream> input, std::filesystem::path const& assetName) : BinaryReader(std::move(input)) {
		this->contentManager = contentManager;
		this->assetName = assetName;
	}

	ContentReader ContentReader::Create(Xna::ContentManager* contentManager, std::unique_ptr<Stream> input, std::filesystem::path const& assetName) {
		PrepareStream(input.get(), assetName);

		return ContentReader(contentManager, std::move(input), assetName);
	}

	void ContentReader::PrepareStream(Stream* input, std::filesystem::path const& assetName) {
		XNPP_CORE_ASSERT(input->CanSeek(), "Invalid cannot seek.");

		auto binaryReader = BinaryReader(input);

		const auto x = binaryReader.ReadByte() == 88;
		const auto n = binaryReader.ReadByte() == 78;
		const auto b = binaryReader.ReadByte() == 66;

		XNPP_CORE_ASSERT(x && n && b, "Invalid XNB file.");

		int32_t num1 = 0;
		const auto w = binaryReader.ReadByte() == PlatformLabel;

		XNPP_CORE_ASSERT(w, "Invalid XNB file.");

		num1 = binaryReader.ReadUInt16();

		//Descartado
		auto graphicsProfile = (num1 & XnbVersionProfileMask) >> XnbVersionProfileShift;
		bool compress = false;

		switch (num1 & -32513)
		{
		case XnbVersion:
			break;
		case XnbCompressedVersion:
			compress = true;
			break;
		default:
			XNPP_CORE_ASSERT(false, "Invalid XNB file.");
			break;
		}

		auto num2 = binaryReader.ReadInt32();

		const auto inLen = input->Length();
		const auto inPos = input->Position();

		XNPP_CORE_ASSERT(!(static_cast<int64_t>(num2 - XnbPrologueSize) > (inLen - inPos)), "Invalid XNB file.");

		if (!compress)
			return;

		auto compressedTodo = num2 - XnbCompressedPrologueSize;
		auto decompressedTodo = binaryReader.ReadInt32();

		XNPP_CORE_ASSERT(false, "Decompress Stream is not supported.");
		//TODO:
		//stream = DecompressStream(input, compressedTodo, decompressedTodo);
	}

	int32_t ContentReader::ReadHeader() {
		const auto encodedInt = Read7BitEncodedInt();
		auto typeReaderManager = contentManager->GetTypeReaderManager();
		typeReaders = typeReaderManager->ReadTypeManifest(encodedInt, this);

		const auto length = Read7BitEncodedInt();

		if (length > 0) {
			//TODO:: há algo a fazer?
		}

		return length;
	}

	const std::vector<uint8_t>& ContentReader::ReadByteBuffer(size_t size) {
		auto& buffer = contentManager->byteBuffer;

		if (buffer.size() < size)
			buffer.resize(size);

		int32_t num{ 0 };

		for (size_t index = 0; index < size; index += num)
		{
			auto span = std::span{ buffer }.subspan(index, size - index);

			num = Read(span);

			XNPP_CORE_ASSERT(num != 0, "Bad xnb file. ReadByteBuffer failed.");
		}

		return buffer;
	}

	Vector2 ContentReader::ReadVector2() {
		Vector2 vector2;
		vector2.X = ReadSingle();
		vector2.Y = ReadSingle();
		return vector2;
	}

	Vector3 ContentReader::ReadVector3() {
		Vector3 vector3;
		vector3.X = ReadSingle();
		vector3.Y = ReadSingle();
		vector3.Z = ReadSingle();
		return vector3;
	}

	Vector4 ContentReader::ReadVector4() {
		Vector4 vector4;
		vector4.X = ReadSingle();
		vector4.Y = ReadSingle();
		vector4.Z = ReadSingle();
		vector4.W = ReadSingle();
		return vector4;
	}

	Matrix ContentReader::ReadMatrix() {
		Matrix matrix;
		matrix.M11 = ReadSingle();
		matrix.M12 = ReadSingle();
		matrix.M13 = ReadSingle();
		matrix.M14 = ReadSingle();
		matrix.M21 = ReadSingle();
		matrix.M22 = ReadSingle();
		matrix.M23 = ReadSingle();
		matrix.M24 = ReadSingle();
		matrix.M31 = ReadSingle();
		matrix.M32 = ReadSingle();
		matrix.M33 = ReadSingle();
		matrix.M34 = ReadSingle();
		matrix.M41 = ReadSingle();
		matrix.M42 = ReadSingle();
		matrix.M43 = ReadSingle();
		matrix.M44 = ReadSingle();
		return matrix;
	}
	Quaternion ContentReader::ReadQuaternion() {
		Quaternion quaternion;
		quaternion.X = ReadSingle();
		quaternion.Y = ReadSingle();
		quaternion.Z = ReadSingle();
		quaternion.W = ReadSingle();
		return quaternion;
	}

	std::filesystem::path ContentReader::GetPathToReference(std::filesystem::path const& referenceName) const {
		// 1. Criamos um objeto path a partir da string assetName
		std::filesystem::path pAsset(assetName);

		// 2. p.parent_path() extrai tudo até a última barra (o diretório)
		// 3. O operador / faz o papel do Path.Combine do C#
		const auto result = pAsset.parent_path() / referenceName;

		// 4. Retornamos como string (formato nativo do SO)
		return result;
	}

	std::filesystem::path ContentReader::GetAbsolutePathToReference(std::filesystem::path const& referenceName) const {
		auto reference = referenceName;
		reference = GetPathToReference(referenceName);
		reference = CSharp::Path::Combine(contentManager->fullRootDirectory, reference);

		auto location = TitleLocation::Path();
		location.remove_filename();

		reference = CSharp::Path::Combine(location, reference);

		return TitleContainer::GetCleanPath(reference);
	}
}

//////////////////////////////////////////////////////////////////
// CONTENT TYPE READER MANAGER									//
//////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////
// DEFAULT CONTENT READERS										//
//////////////////////////////////////////////////////////////////
namespace Xna {
	BoundingBox BoundingBoxReader::Read(ContentReader& input, BoundingBox& existingInstance) {
		BoundingBox boundingBox;
		boundingBox.Min = input.ReadVector3();
		boundingBox.Max = input.ReadVector3();
		return boundingBox;
	}

	BoundingSphere BoundingSphereReader::Read(ContentReader& input, BoundingSphere& existingInstance) {
		BoundingSphere boundingSphere;
		boundingSphere.Center = input.ReadVector3();
		boundingSphere.Radius = input.ReadSingle();
		return boundingSphere;
	}

	Curve CurveReader::Read(ContentReader& input, Curve& existingInstance) {
		Curve curve = existingInstance;
		curve.PreLoop = static_cast<CurveLoopType>(input.ReadInt32());
		curve.PostLoop = static_cast<CurveLoopType>(input.ReadInt32());
		const auto num1 = input.ReadInt32();

		for (size_t index = 0; index < num1; ++index) {
			const auto position = input.ReadSingle();
			const auto num2 = input.ReadSingle();
			const auto tangentIn = input.ReadSingle();
			const auto tangentOut = input.ReadSingle();
			CurveContinuity continuity = static_cast<CurveContinuity>(input.ReadInt32());
			curve.Keys.Add(CurveKey(position, num2, tangentIn, tangentOut, continuity));
		}
		return curve;
	}

	Plane PlaneReader::Read(ContentReader& input, Plane& existingInstance) {
		Plane plane;
		plane.Normal = input.ReadVector3();
		plane.D = input.ReadSingle();
		return plane;
	}

	Point PointReader::Read(ContentReader& input, Point& existingInstance) {
		Point point;
		point.X = input.ReadInt32();
		point.Y = input.ReadInt32();
		return point;
	}

	Ray RayReader::Read(ContentReader& input, Ray& existingInstance) {
		Ray ray;
		ray.Position = input.ReadVector3();
		ray.Direction = input.ReadVector3();
		return ray;
	}

	Rectangle RectangleReader::Read(ContentReader& input, Rectangle& existingInstance) {
		Rectangle rectangle;
		rectangle.X = input.ReadInt32();
		rectangle.Y = input.ReadInt32();
		rectangle.Width = input.ReadInt32();
		rectangle.Height = input.ReadInt32();
		return rectangle;
	}

	Song SongReader::Read(ContentReader& input, Song& existingInstance) {
		const auto referenceName = input.ReadString();
		const auto absolutePathToReference = input.GetAbsolutePathToReference(referenceName);
		const auto duration = input.ReadObject<int32_t>();

		return Song(input.AssetName().string(), absolutePathToReference, duration);
	}

	SoundEffect SoundEffectReader::Read(ContentReader& input, SoundEffect& existingInstance) {
		const auto count1 = input.ReadInt32();
		const auto format = input.ReadBytes(count1);
		const auto count2 = input.ReadInt32();
		const auto data = input.ReadBytes(count2);
		const auto loopStart = input.ReadInt32();
		const auto loopLength = input.ReadInt32();
		const auto num = input.ReadInt32();
		return SoundEffect(format, data, loopStart, loopLength, TimeSpan::FromMilliseconds(static_cast<double>(num)));
	}

	Texture2D Texture2DReader::Read(ContentReader& input, Texture2D& existingInstance) {
		SurfaceFormat format = static_cast<SurfaceFormat>(input.ReadInt32());
		const auto width = input.ReadInt32();
		const auto height = input.ReadInt32();
		const auto num1 = input.ReadInt32();
		auto texture2D = Texture2D(GraphicsContentHelper::GraphicsDeviceFromContentReader(input), width, height, num1 > 1, format);

		for (size_t level = 0; level < num1; ++level)
		{
			const auto num2 = input.ReadInt32();
			const auto& data = input.ReadByteBuffer(num2);
			texture2D.SetData(level, std::nullopt, data, 0, num2);
		}

		return texture2D;
	}

	SpriteFont SpriteFontReader::Read(ContentReader& input, SpriteFont& existingInstance) {
		auto texture = input.ReadObject<Texture2D>();
		auto glyphs = input.ReadObject<std::vector<Rectangle>>();
		auto cropping = input.ReadObject<std::vector<Rectangle>>();
		auto charMap = input.ReadObject<std::vector<uint32_t>>();
		const auto lineSpacing = input.ReadInt32();
		const auto spacing = input.ReadSingle();
		auto kerning = input.ReadObject<std::vector<Vector3>>();
		std::optional<char32_t> defaultCharacter = std::nullopt;

		if (input.ReadBoolean())
			defaultCharacter = static_cast<char32_t>(input.ReadUInt32()); //input.ReadChar

		std::vector<char32_t> chars(charMap.size());

		for (size_t i = 0; i < charMap.size(); ++i)
			chars[i] = static_cast<char32_t>(charMap[i]);

		return SpriteFont(texture, glyphs, cropping, chars, lineSpacing, spacing, kerning, defaultCharacter);
	}
}