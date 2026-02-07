#ifndef XNA_BASICREADERS_HPP
#define XNA_BASICREADERS_HPP

#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/IO/BinaryReader.hpp"
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/Framework/Audio/SoundEffect.hpp"
#include "Xna/Framework/BoundingBox.hpp"
#include "Xna/Framework/BoundingSphere.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Framework/Content/ContentReader.hpp"
#include "Xna/Framework/Content/ContentTypeReader.hpp"
#include "Xna/Framework/Curve.hpp"
#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Media/Song.hpp"
#include "Xna/Framework/Plane.hpp"
#include "Xna/Framework/Point.hpp"
#include "Xna/Framework/Quaternion.hpp"
#include "Xna/Framework/Ray.hpp"
#include "Xna/Framework/Rectangle.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Vector4.hpp"
#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include <any>
#include <cstdint>
#include <string>
#include <vector>

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
		ObjectReader() : ContentTypeReader(CSharp::typeof<std::any>()) {}
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

	struct TimeSpanReader : public ContentTypeReaderT<CSharp::TimeSpan> {
	protected:
		inline CSharp::TimeSpan Read(ContentReader& input, CSharp::TimeSpan& existingInstance) override {
			return CSharp::TimeSpan::FromTicks(input.ReadInt64());
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
}

#endif