#include <Xna/CSharp/Exception.hpp>
#include "Xna/Framework/Content/ContentManager.hpp"
#include <Xna/CSharp/Path.hpp>
#include "../TitleLocation.hpp"
#include "Xna/Framework/TitleContainer.hpp"

using Xna::CSharp::InvalidOperationException;

namespace Xna {
	ContentReader::ContentReader(Xna::ContentManager* contentManager, Stream* input, std::filesystem::path const& assetName, int32_t graphicsProfile) : BinaryReader(input) {
		this->contentManager = contentManager;
		this->assetName = assetName;
		this->graphicsProfile = graphicsProfile;
	}

	ContentReader ContentReader::Create(Xna::ContentManager* contentManager, Stream* input, std::filesystem::path const& assetName) {

		int32_t graphicsProfile;
		PrepareStream(input, assetName, graphicsProfile);

		return ContentReader(contentManager, input, assetName, graphicsProfile);
	}

	void ContentReader::PrepareStream(Stream* input, std::filesystem::path const& assetName, int32_t& graphicsProfile) {

		auto binaryReader = BinaryReader(input);

		if (binaryReader.ReadByte() != 88 || binaryReader.ReadByte() != 78 || binaryReader.ReadByte() != 66)
			throw InvalidOperationException("Bad Xnb");

		int32_t num1 = 0;

		if (binaryReader.ReadByte() == 119)
			num1 = binaryReader.ReadUInt16();
		else
			throw InvalidOperationException("Bad Xnb");

		graphicsProfile = (num1 & 32512) >> 8;
		bool flag = false;

		switch (num1 & -32513)
		{
		case 5:
			break;
		case 32773:
			flag = true;
		default:
			throw InvalidOperationException("Bad Xnb");
		}

		auto num2 = binaryReader.ReadInt32();

		const auto inLen = input->Length();
		const auto inPos = input->Position();

		if (input->CanSeek() && (num2 - 10) > (inLen - inPos))
			throw InvalidOperationException("Bad Xnb");

		if (!flag)
			return;

		auto compressedTodo = num2 - 14;
		auto decompressedTodo = binaryReader.ReadInt32();

		throw CSharp::NotSupportedException("DecompressStream is not supported.");
		//TODO:
		//stream = DecompressStream(input, compressedTodo, decompressedTodo);
	}

	int32_t ContentReader::ReadHeader() {
		const auto encodedInt = Read7BitEncodedInt();
		typeReaders = ContentTypeReaderManager::ReadTypeManifest(encodedInt, *this);

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
		
		int32_t num{0};
		
		for (size_t index = 0; index < size; index += num)
		{
			auto span = std::span{ buffer }.subspan(index, size - index);

			num = Read(span);
			
			if (num == 0)
				throw CSharp::InvalidOperationException("BadXnb");
		}

		return buffer;
	}

	Vector2 ContentReader::ReadVector2(){
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

	Vector4 ContentReader::ReadVector4(){
		Vector4 vector4;
		vector4.X = ReadSingle();
		vector4.Y = ReadSingle();
		vector4.Z = ReadSingle();
		vector4.W = ReadSingle();
		return vector4;
	}

	Matrix ContentReader::ReadMatrix(){
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
	Quaternion ContentReader::ReadQuaternion(){
		Quaternion quaternion;
		quaternion.X = ReadSingle();
		quaternion.Y = ReadSingle();
		quaternion.Z = ReadSingle();
		quaternion.W = ReadSingle();
		return quaternion;
	}

	std::filesystem::path ContentReader::GetPathToReference(std::filesystem::path const& referenceName) {
		// 1. Criamos um objeto path a partir da string assetName
		std::filesystem::path pAsset(assetName);

		// 2. p.parent_path() extrai tudo até a última barra (o diretório)
		// 3. O operador / faz o papel do Path.Combine do C#
		const auto result = pAsset.parent_path() / referenceName;

		// 4. Retornamos como string (formato nativo do SO)
		return result;
	}

	std::filesystem::path ContentReader::GetAbsolutePathToReference(std::filesystem::path const& referenceName) {
		auto reference = referenceName;
		reference = GetPathToReference(referenceName);
		reference = CSharp::Path::Combine(contentManager->fullRootDirectory, reference);

		auto location = TitleLocation::Path();
		location.remove_filename();

		reference = CSharp::Path::Combine(location, reference);

		return TitleContainer::GetCleanPath(reference);
	}
}