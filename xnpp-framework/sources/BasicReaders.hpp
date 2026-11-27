#pragma once

#include <Xna/CSharp/Exception.hpp>
#include <Xna/CSharp/IO.hpp>
#include <Xna/CSharp/TimeSpan.hpp>
#include "Xna/Framework/Audio/SoundEffect.hpp"
#include "Xna/Framework/Volume.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Framework/Content/ContentManager.hpp"
#include "Xna/Framework/Curve.hpp"
#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/MathTypes.hpp"
#include "Xna/Framework/Media/Song.hpp"
#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include <any>
#include <cstdint>
#include <string>
#include <vector>
#include <cstddef>
#include <optional>
#include <unordered_map>

namespace Xna {
	struct BooleanReader : public ContentTypeReaderT<bool> {
	protected:
		inline bool Read(ContentReader& input, bool& existingInstance) override {
			return input.ReadBoolean();
		}
	};

	struct ByteReader : public ContentTypeReaderT<uint8_t> {
	protected:
		inline uint8_t Read(ContentReader& input, uint8_t& existingInstance) override {
			return input.ReadByte();
		}
	};

	struct SByteReader : public ContentTypeReaderT<int8_t> {
	protected:
		inline int8_t Read(ContentReader& input, int8_t& existingInstance) override {
			return input.ReadSByte();
		}
	};

	struct CharReader : public ContentTypeReaderT<char32_t> {
	protected:
		inline char32_t Read(ContentReader& input, char32_t& existingInstance) override {
			return input.ReadByte();
		}
	};

	struct ColorReader : public ContentTypeReaderT<Color> {
	protected:
		inline Color Read(ContentReader& input, Color& existingInstance) override {
			return Color(input.ReadUInt32());
		}
	};

	struct CurveReader : public ContentTypeReaderT<Curve> {
	protected:
		Curve Read(ContentReader& input, Curve& existingInstance) override;

	public:
		bool CanDeserializeIntoExistingObject() const override {
			return true;
		}
	};

	struct BoundingBoxReader : public ContentTypeReaderT<BoundingBox> {
	protected:
		BoundingBox Read(ContentReader& input, BoundingBox& existingInstance) override;
	};

	struct BoundingSphereReader : public ContentTypeReaderT<BoundingSphere> {
	protected:
		BoundingSphere Read(ContentReader& input, BoundingSphere& existingInstance) override;
	};

	struct DoubleReader : public ContentTypeReaderT<double> {
	protected:
		inline double Read(ContentReader& input, double& existingInstance) override {
			return input.ReadDouble();
		}
	};

	struct SingleReader : public ContentTypeReaderT<float> {
	protected:
		inline float Read(ContentReader& input, float& existingInstance) override {
			return input.ReadSingle();
		}
	};

	struct Int16Reader : public ContentTypeReaderT<int16_t> {
	protected:
		inline int16_t Read(ContentReader& input, int16_t& existingInstance) override {
			return input.ReadInt16();
		}
	};

	struct UInt16Reader : public ContentTypeReaderT<uint16_t> {
	protected:
		inline uint16_t Read(ContentReader& input, uint16_t& existingInstance) override {
			return input.ReadUInt16();
		}
	};

	struct Int32Reader : public ContentTypeReaderT<int32_t> {
	protected:
		inline int32_t Read(ContentReader& input, int32_t& existingInstance) override {
			return input.ReadInt32();
		}
	};

	struct UInt32Reader : public ContentTypeReaderT<uint32_t> {
	protected:
		inline uint32_t Read(ContentReader& input, uint32_t& existingInstance) override {
			return input.ReadUInt32();
		}
	};

	struct Int64Reader : public ContentTypeReaderT<int64_t> {
	protected:
		inline int64_t Read(ContentReader& input, int64_t& existingInstance) override {
			return input.ReadInt64();
		}
	};

	struct UInt64Reader : public ContentTypeReaderT<uint64_t> {
	protected:
		inline uint64_t Read(ContentReader& input, uint64_t& existingInstance) override {
			return input.ReadUInt64();
		}
	};

	struct MatrixReader : public ContentTypeReaderT<Matrix> {
	protected:
		inline Matrix Read(ContentReader& input, Matrix& existingInstance) override {
			return input.ReadMatrix();
		}
	};

	struct ObjectReader : public ContentTypeReader {
	public:
		ObjectReader() : ContentTypeReader(CSharp::Typeof<std::any>()) {}
	protected:
		inline std::any Read(ContentReader& input, std::any& existingInstance) override {
			throw CSharp::NotSupportedException();

			return {};
		}
	};

	struct PlaneReader : public ContentTypeReaderT<Plane> {
	protected:
		Plane Read(ContentReader& input, Plane& existingInstance);
	};

	struct PointReader : public ContentTypeReaderT<Point> {
	protected:
		Point Read(ContentReader& input, Point& existingInstance) override;
	};

	struct QuaternionReader : public ContentTypeReaderT<Quaternion> {
	protected:
		inline Quaternion Read(ContentReader& input, Quaternion& existingInstance) override {
			return input.ReadQuaternion();
		}
	};

	struct RayReader : public ContentTypeReaderT<Ray> {
	protected:
		Ray Read(ContentReader& input, Ray& existingInstance) override;
	};

	struct RectangleReader : public ContentTypeReaderT<Rectangle> {
	protected:
		Rectangle Read(ContentReader& input, Rectangle& existingInstance) override;
	};

	struct SongReader : public ContentTypeReaderT<Song> {
	protected:
		Song Read(ContentReader& input, Song& existingInstance) override;
	};

	struct SoundEffectReader : public ContentTypeReaderT<SoundEffect> {
	protected:
		SoundEffect Read(ContentReader& input, SoundEffect& existingInstance) override;
	};

	struct SpriteFontReader : public ContentTypeReaderT<SpriteFont> {
	protected:
		SpriteFont Read(ContentReader& input, SpriteFont& existingInstance) override;
	};

	struct StringReader : public ContentTypeReaderT<std::string> {
	protected:
		inline std::string Read(ContentReader& input, std::string& existingInstance) override {
			return input.ReadString();
		}
	};

	struct Texture2DReader : public ContentTypeReaderT<Texture2D> {
	protected:
		Texture2D Read(ContentReader& input, Texture2D& existingInstance) override;
	};

	struct TimeSpanReader : public ContentTypeReaderT<TimeSpan> {
	protected:
		inline TimeSpan Read(ContentReader& input, TimeSpan& existingInstance) override {
			return TimeSpan::FromTicks(input.ReadInt64());
		}
	};

	struct Vector2Reader : public ContentTypeReaderT<Vector2> {
	protected:
		inline Vector2 Read(ContentReader& input, Vector2& existingInstance) override {
			return input.ReadVector2();
		}
	};

	struct Vector3Reader : public ContentTypeReaderT<Vector3> {
	protected:
		inline Vector3 Read(ContentReader& input, Vector3& existingInstance) override {
			return input.ReadVector3();
		}
	};

	struct Vector4Reader : public ContentTypeReaderT<Vector4> {
	protected:
		inline Vector4 Read(ContentReader& input, Vector4& existingInstance) override {
			return input.ReadVector4();
		}
	};

	template <typename T>
	struct ArrayReader : public ContentTypeReaderT<std::vector<T>> {
	protected:
		void Initialize(ContentTypeReaderManager& manager) override {
			elementReader = manager.GetTypeReader(CSharp::Typeof<T>());
		}

		std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			const auto length = input.ReadInt32();
			std::vector<T> objArray(static_cast<size_t>(length));

			for (size_t index = 0; index < length; ++index)
				objArray[index] = input.ReadObject<T>(*elementReader);

			return objArray;
		}

	private:
		ContentTypeReader* elementReader;
	};

	template <typename Key, typename Value>
	struct DictionaryReader : public ContentTypeReaderT<std::unordered_map<Key, Value>> {
	protected:
		void Initialize(ContentTypeReaderManager& manager) override {
			keyReader = manager.GetTypeReader(CSharp::Typeof<Key>());
			valueReader = manager.GetTypeReader(CSharp::Typeof<Value>());
		}

		inline std::unordered_map<Key, Value> Read(ContentReader& input, std::unordered_map<Key, Value>& existingInstance) override {
			auto capacity = input.ReadInt32();
			auto& dictionary = existingInstance;

			while (capacity-- > 0)
			{
				const auto key = input.ReadObject<Key>(*keyReader);
				const auto obj = input.ReadObject<Value>(*valueReader);
				dictionary.emplace(key, obj);
			}
			return dictionary;
		}

	private:
		ContentTypeReader* keyReader;
		ContentTypeReader* valueReader;
	};

	template <typename T>
	struct ListReader : public ContentTypeReaderT<std::vector<T>> {
	protected:
		inline void Initialize(ContentTypeReaderManager& manager) override {
			elementReader = manager.GetTypeReader(CSharp::Typeof<T>());
		}

		inline std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			auto num = input.ReadInt32();
			auto& objList = existingInstance;

			while (num-- > 0)
				objList.push_back(input.ReadObject<T>(*elementReader));

			return objList;
		}

	private:
		ContentTypeReader* elementReader;

	public:
		bool CanDeserializeIntoExistingObject() const override { return true; }
	};

	template <typename T>
	struct NullableReader : public ContentTypeReaderT<std::optional<T>> {
	protected:
		inline void Initialize(ContentTypeReaderManager& manager) override {
			underlyingTypeReader = manager.GetTypeReader(CSharp::Typeof<T>());
		}

		inline std::optional<T> Read(ContentReader& input, std::optional<T>& existingInstance) override {
			if (!input.ReadBoolean())
				return std::nullopt;

			return input.ReadRawObject<T>(*underlyingTypeReader);
		}

	private:
		ContentTypeReader* underlyingTypeReader;
	};
}
