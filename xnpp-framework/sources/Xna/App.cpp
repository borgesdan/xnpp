#include "Framework/Content/Readers/BasicReaders.hpp"
#include "Xna/Framework/Content/Readers/ArrayReader.hpp"
#include "Xna/Framework/Content/Readers/DictionaryReader.hpp"
#include "Xna/Framework/Content/Readers/ListReader.hpp"
#include "Xna/Framework/Content/Readers/NullableReader.hpp"
#include "Xna/Internal/App.hpp"

namespace Xna {
	TypeRegistry<ContentTypeReader>& App::GetContentTypeReaderActivator() {
		static TypeRegistry<ContentTypeReader> ContentTypeReaderActivator1;
		return ContentTypeReaderActivator1;
	}

	void App::RegisterContentTypeReaders() {
		GetContentTypeReaderActivator().Add(ArrayReader<bool>());
		GetContentTypeReaderActivator().Add(ArrayReader<char>());
		GetContentTypeReaderActivator().Add(ArrayReader<int>());
		GetContentTypeReaderActivator().Add(ArrayReader<float>());
		GetContentTypeReaderActivator().Add(ArrayReader<double>());
		GetContentTypeReaderActivator().Add(ArrayReader<char32_t>());
		GetContentTypeReaderActivator().Add(DictionaryReader<int, int>());
		GetContentTypeReaderActivator().Add(NullableReader<int>());
		GetContentTypeReaderActivator().Add(BooleanReader(), {"BooleanReader", "Microsoft.Xna.Framework.Content.BooleanReader" });
		GetContentTypeReaderActivator().Add(BoundingBoxReader(), {"BoundingBoxReader", "Microsoft.Xna.Framework.Content.BoundingBoxReader" });
		GetContentTypeReaderActivator().Add(BoundingSphereReader(), {"BoundingSphereReader", "Microsoft.Xna.Framework.Content.BoundingSphereReader" });
		GetContentTypeReaderActivator().Add(ByteReader(), { "ByteReader", "Microsoft.Xna.Framework.Content.ByteReader" });
		GetContentTypeReaderActivator().Add(SByteReader(), { "SByteReader", "Microsoft.Xna.Framework.Content.SByteReader" });
		GetContentTypeReaderActivator().Add(CharReader(), { "CharReader", "Microsoft.Xna.Framework.Content.CharReader" });
		GetContentTypeReaderActivator().Add(ColorReader(), { "ColorReader", "Microsoft.Xna.Framework.Content.ColorReader" });
		GetContentTypeReaderActivator().Add(CurveReader(), { "CurveReader", "Microsoft.Xna.Framework.Content.SingleReader" });
		GetContentTypeReaderActivator().Add(DoubleReader(), { "DoubleReader", "Microsoft.Xna.Framework.Content.DoubleReader" });
		GetContentTypeReaderActivator().Add(SingleReader(), { "SingleReader", "Microsoft.Xna.Framework.Content.SingleReader" });
		GetContentTypeReaderActivator().Add(Int16Reader(), { "Int16Reader", "Microsoft.Xna.Framework.Content.Int16Reader" });
		GetContentTypeReaderActivator().Add(Int32Reader(), { "Int32Reader", "Microsoft.Xna.Framework.Content.Int32Reader" });
		GetContentTypeReaderActivator().Add(Int64Reader(), { "Int64Reader", "Microsoft.Xna.Framework.Content.Int64Reader" });
		GetContentTypeReaderActivator().Add(UInt16Reader(), { "UInt16Reader", "Microsoft.Xna.Framework.Content.UInt16Reader" });
		GetContentTypeReaderActivator().Add(UInt32Reader(), { "UInt32Reader", "Microsoft.Xna.Framework.Content.UInt32Reader" });
		GetContentTypeReaderActivator().Add(UInt64Reader(), { "UInt64Reader", "Microsoft.Xna.Framework.Content.UInt64Reader" });
		GetContentTypeReaderActivator().Add(MatrixReader(), { "MatrixReader", "Microsoft.Xna.Framework.Content.MatrixReader" });
		GetContentTypeReaderActivator().Add(ObjectReader(), { "ObjectReader", "Microsoft.Xna.Framework.Content.ObjectReader" });
		GetContentTypeReaderActivator().Add(PlaneReader(), { "PlaneReader", "Microsoft.Xna.Framework.Content.PlaneReader" });
		GetContentTypeReaderActivator().Add(PointReader(), { "PointReader", "Microsoft.Xna.Framework.Content.PointReader" });
		GetContentTypeReaderActivator().Add(QuaternionReader(), { "QuaternionReader", "Microsoft.Xna.Framework.Content.QuaternionReader" });
		GetContentTypeReaderActivator().Add(RayReader(), { "RayReader", "Microsoft.Xna.Framework.Content.RayReader" });
		GetContentTypeReaderActivator().Add(RectangleReader(), { "RectangleReader", "Microsoft.Xna.Framework.Content.RectangleReader" });
		GetContentTypeReaderActivator().Add(StringReader(), { "StringReader", "Microsoft.Xna.Framework.Content.StringReader" });
		GetContentTypeReaderActivator().Add(TimeSpanReader(), { "TimeSpanReader", "Microsoft.Xna.Framework.Content.TimeSpanReader" });
		GetContentTypeReaderActivator().Add(Vector2Reader(), { "Vector2Reader", "Microsoft.Xna.Framework.Content.Vector2Reader" });
		GetContentTypeReaderActivator().Add(Vector3Reader(), { "Vector3Reader", "Microsoft.Xna.Framework.Content.Vector3Reader" });
		GetContentTypeReaderActivator().Add(Vector4Reader(), { "Vector4Reader", "Microsoft.Xna.Framework.Content.Vector4Reader" });
		GetContentTypeReaderActivator().Add(Texture2DReader(), { "Texture2DReader", "Microsoft.Xna.Framework.Content.Texture2DReader" });
		GetContentTypeReaderActivator().Add(SoundEffectReader(), { "SoundEffectReader", "Microsoft.Xna.Framework.Content.SoundEffectReader" });
		GetContentTypeReaderActivator().Add(SongReader(), { "SongReader", "Microsoft.Xna.Framework.Content.SongReader" });
		GetContentTypeReaderActivator().Add(SpriteFontReader(), { "SpriteFontReader", "Microsoft.Xna.Framework.Content.SongReader" });

		GetContentTypeReaderActivator().Add(ListReader<bool>(), { "ListReader<bool>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Boolean" });
		GetContentTypeReaderActivator().Add(ListReader<char>(), { "ListReader<char>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		GetContentTypeReaderActivator().Add(ListReader<uint8_t>(), { "ListReader<uint8_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Byte" });
		GetContentTypeReaderActivator().Add(ListReader<float>(), { "ListReader<float>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Float" });
		GetContentTypeReaderActivator().Add(ListReader<double>(), { "ListReader<double>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Single" });
		GetContentTypeReaderActivator().Add(ListReader<char32_t>(), { "ListReader<char32_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		GetContentTypeReaderActivator().Add(ListReader<int16_t>(), { "ListReader<int16_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int16" });
		GetContentTypeReaderActivator().Add(ListReader<int32_t>(), { "ListReader<int32_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int32" });
		GetContentTypeReaderActivator().Add(ListReader<int64_t>(), { "ListReader<int64_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int64" });
		GetContentTypeReaderActivator().Add(ListReader<uint16_t>(), { "ListReader<uint16_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt16" });
		GetContentTypeReaderActivator().Add(ListReader<uint32_t>(), { "ListReader<uint32_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt32" });
		GetContentTypeReaderActivator().Add(ListReader<uint64_t>(), { "ListReader<uint64_t>", "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt64" });
		GetContentTypeReaderActivator().Add(ListReader<Rectangle>(), { "ListReader<Rectangle>", "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Rectangle" });
		GetContentTypeReaderActivator().Add(ListReader<Vector2>(), { "ListReader<Vector2>", "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector2" });
		GetContentTypeReaderActivator().Add(ListReader<Vector3>(), { "ListReader<Vector3>", "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector3" });
		GetContentTypeReaderActivator().Add(ListReader<Vector4>(), { "ListReader<Vector4>", "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector4" });
	};
}