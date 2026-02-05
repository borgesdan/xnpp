#include "Xna/App.hpp"
#include "Xna/Framework/Content/ContentTypeActivator.hpp"
#include "Xna/Framework/Content/Readers/ArrayReader.hpp"
#include "Xna/Framework/Content/Readers/BooleanReader.hpp"
#include "Xna/Framework/Content/Readers/BoundingBoxReader.hpp"
#include "Xna/Framework/Content/Readers/BoundingSphereReader.hpp"
#include "Xna/Framework/Content/Readers/ByteReader.hpp"
#include "Xna/Framework/Content/Readers/CharReader.hpp"
#include "Xna/Framework/Content/Readers/ColorReader.hpp"
#include "Xna/Framework/Content/Readers/CurveReader.hpp"
#include "Xna/Framework/Content/Readers/DoubleReader.hpp"
#include "Xna/Framework/Content/Readers/DictionaryReader.hpp"
#include "Xna/Framework/Content/Readers/IntReader.hpp"
#include "Xna/Framework/Content/Readers/ListReader.hpp"
#include "Xna/Framework/Content/Readers/MatrixReader.hpp"
#include "Xna/Framework/Content/Readers/NullableReader.hpp"
#include "Xna/Framework/Content/Readers/ObjectReader.hpp"
#include "Xna/Framework/Content/Readers/PlaneReader.hpp"
#include "Xna/Framework/Content/Readers/PointReader.hpp"
#include "Xna/Framework/Content/Readers/QuaternionReader.hpp"
#include "Xna/Framework/Content/Readers/RayReader.hpp"
#include "Xna/Framework/Content/Readers/RectangleReader.hpp"
#include "Xna/Framework/Content/Readers/StringReader.hpp"
#include "Xna/Framework/Content/Readers/TimeSpanReader.hpp"
#include "Xna/Framework/Content/Readers/VectorReader.hpp"
#include "Xna/Framework/Content/Readers/Texture2DReader.hpp"
#include "Xna/Framework/Content/Readers/SoundEffectReader.hpp"
#include "Xna/Framework/Content/Readers/SongReader.hpp"

namespace Xna {
#define REGISTER_READER(ReaderInst, FullName) \
    GetContentTypeReaderActivator().Add(ReaderInst, { #ReaderInst, FullName })

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
		REGISTER_READER(BooleanReader(), { "Microsoft.Xna.Framework.Content.BooleanReader" });
		REGISTER_READER(BoundingBoxReader(), { "Microsoft.Xna.Framework.Content.BoundingBoxReader" });
		REGISTER_READER(BoundingSphereReader(), { "Microsoft.Xna.Framework.Content.BoundingSphereReader" });
		REGISTER_READER(ByteReader(), { "Microsoft.Xna.Framework.Content.ByteReader" });
		REGISTER_READER(SByteReader(), { "Microsoft.Xna.Framework.Content.SByteReader" });
		REGISTER_READER(CharReader(), { "Microsoft.Xna.Framework.Content.CharReader" });
		REGISTER_READER(ColorReader(), { "Microsoft.Xna.Framework.Content.ColorReader" });
		REGISTER_READER(CurveReader(), { "Microsoft.Xna.Framework.Content.SingleReader" });
		REGISTER_READER(DoubleReader(), { "Microsoft.Xna.Framework.Content.DoubleReader" });
		REGISTER_READER(SingleReader(), { "Microsoft.Xna.Framework.Content.SingleReader" });
		REGISTER_READER(Int16Reader(), { "Microsoft.Xna.Framework.Content.Int16Reader" });
		REGISTER_READER(Int32Reader(), { "Microsoft.Xna.Framework.Content.Int32Reader" });
		REGISTER_READER(Int64Reader(), { "Microsoft.Xna.Framework.Content.Int64Reader" });
		REGISTER_READER(UInt16Reader(), { "Microsoft.Xna.Framework.Content.UInt16Reader" });
		REGISTER_READER(UInt32Reader(), { "Microsoft.Xna.Framework.Content.UInt32Reader" });
		REGISTER_READER(UInt64Reader(), { "Microsoft.Xna.Framework.Content.UInt64Reader" });
		REGISTER_READER(MatrixReader(), { "Microsoft.Xna.Framework.Content.MatrixReader" });
		REGISTER_READER(ObjectReader(), { "Microsoft.Xna.Framework.Content.ObjectReader" });
		REGISTER_READER(PlaneReader(), { "Microsoft.Xna.Framework.Content.PlaneReader" });
		REGISTER_READER(PointReader(), { "Microsoft.Xna.Framework.Content.PointReader" });
		REGISTER_READER(QuaternionReader(), { "Microsoft.Xna.Framework.Content.QuaternionReader" });
		REGISTER_READER(RayReader(), { "Microsoft.Xna.Framework.Content.RayReader" });
		REGISTER_READER(RectangleReader(), { "Microsoft.Xna.Framework.Content.RectangleReader" });
		REGISTER_READER(StringReader(), { "Microsoft.Xna.Framework.Content.StringReader" });
		REGISTER_READER(TimeSpanReader(), { "Microsoft.Xna.Framework.Content.TimeSpanReader" });
		REGISTER_READER(Vector2Reader(), { "Microsoft.Xna.Framework.Content.Vector2Reader" });
		REGISTER_READER(Vector3Reader(), { "Microsoft.Xna.Framework.Content.Vector3Reader" });
		REGISTER_READER(Vector4Reader(), { "Microsoft.Xna.Framework.Content.Vector4Reader" });
		REGISTER_READER(Texture2DReader(), { "Microsoft.Xna.Framework.Content.Texture2DReader" });
		REGISTER_READER(SoundEffectReader(), { "Microsoft.Xna.Framework.Content.SoundEffectReader" });
		REGISTER_READER(SongReader(), { "Microsoft.Xna.Framework.Content.SongReader" });
		REGISTER_READER(ListReader<bool>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Boolean" });
		REGISTER_READER(ListReader<char>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		REGISTER_READER(ListReader<uint8_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Byte" });
		REGISTER_READER(ListReader<float>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Float" });
		REGISTER_READER(ListReader<double>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Single" });
		REGISTER_READER(ListReader<char32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		REGISTER_READER(ListReader<int16_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int16" });
		REGISTER_READER(ListReader<int32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int32" });
		REGISTER_READER(ListReader<int64_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int64" });
		REGISTER_READER(ListReader<uint16_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt16" });
		REGISTER_READER(ListReader<uint32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt32" });
		REGISTER_READER(ListReader<uint64_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt64" });
		REGISTER_READER(ListReader<Rectangle>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Rectangle" });
		REGISTER_READER(ListReader<Vector2>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector2" });
		REGISTER_READER(ListReader<Vector3>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector3" });
		REGISTER_READER(ListReader<Vector4>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector4" });
	};
}