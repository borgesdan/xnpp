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
		GetContentTypeReaderActivator().Add(BooleanReader(), { "Microsoft.Xna.Framework.Content.BooleanReader" });
		GetContentTypeReaderActivator().Add(BoundingBoxReader(), { "Microsoft.Xna.Framework.Content.BoundingBoxReader" });
		GetContentTypeReaderActivator().Add(BoundingSphereReader(), { "Microsoft.Xna.Framework.Content.BoundingSphereReader" });
		GetContentTypeReaderActivator().Add(ByteReader(), { "Microsoft.Xna.Framework.Content.ByteReader" });
		GetContentTypeReaderActivator().Add(SByteReader(), { "Microsoft.Xna.Framework.Content.SByteReader" });
		GetContentTypeReaderActivator().Add(CharReader(), { "Microsoft.Xna.Framework.Content.CharReader" });
		GetContentTypeReaderActivator().Add(ColorReader(), { "Microsoft.Xna.Framework.Content.ColorReader" });
		GetContentTypeReaderActivator().Add(CurveReader(), { "Microsoft.Xna.Framework.Content.SingleReader" });
		GetContentTypeReaderActivator().Add(DoubleReader(), { "Microsoft.Xna.Framework.Content.DoubleReader" });
		GetContentTypeReaderActivator().Add(SingleReader(), { "Microsoft.Xna.Framework.Content.SingleReader" });
		GetContentTypeReaderActivator().Add(Int16Reader(), { "Microsoft.Xna.Framework.Content.Int16Reader" });
		GetContentTypeReaderActivator().Add(Int32Reader(), { "Microsoft.Xna.Framework.Content.Int32Reader" });
		GetContentTypeReaderActivator().Add(Int64Reader(), { "Microsoft.Xna.Framework.Content.Int64Reader" });
		GetContentTypeReaderActivator().Add(UInt16Reader(), { "Microsoft.Xna.Framework.Content.UInt16Reader" });
		GetContentTypeReaderActivator().Add(UInt32Reader(), { "Microsoft.Xna.Framework.Content.UInt32Reader" });
		GetContentTypeReaderActivator().Add(UInt64Reader(), { "Microsoft.Xna.Framework.Content.UInt64Reader" });
		GetContentTypeReaderActivator().Add(MatrixReader(), { "Microsoft.Xna.Framework.Content.MatrixReader" });
		GetContentTypeReaderActivator().Add(ObjectReader(), { "Microsoft.Xna.Framework.Content.ObjectReader" });
		GetContentTypeReaderActivator().Add(PlaneReader(), { "Microsoft.Xna.Framework.Content.PlaneReader" });
		GetContentTypeReaderActivator().Add(PointReader(), { "Microsoft.Xna.Framework.Content.PointReader" });
		GetContentTypeReaderActivator().Add(QuaternionReader(), { "Microsoft.Xna.Framework.Content.QuaternionReader" });
		GetContentTypeReaderActivator().Add(RayReader(), { "Microsoft.Xna.Framework.Content.RayReader" });
		GetContentTypeReaderActivator().Add(RectangleReader(), { "Microsoft.Xna.Framework.Content.RectangleReader" });
		GetContentTypeReaderActivator().Add(StringReader(), { "Microsoft.Xna.Framework.Content.StringReader" });
		GetContentTypeReaderActivator().Add(TimeSpanReader(), { "Microsoft.Xna.Framework.Content.TimeSpanReader" });
		GetContentTypeReaderActivator().Add(Vector2Reader(), { "Microsoft.Xna.Framework.Content.Vector2Reader" });
		GetContentTypeReaderActivator().Add(Vector3Reader(), { "Microsoft.Xna.Framework.Content.Vector3Reader" });
		GetContentTypeReaderActivator().Add(Vector4Reader(), { "Microsoft.Xna.Framework.Content.Vector4Reader" });
		GetContentTypeReaderActivator().Add(Texture2DReader(), { "Microsoft.Xna.Framework.Content.Texture2DReader" });
		GetContentTypeReaderActivator().Add(SoundEffectReader(), { "Microsoft.Xna.Framework.Content.SoundEffectReader" });
		GetContentTypeReaderActivator().Add(SongReader(), { "Microsoft.Xna.Framework.Content.SongReader" });
		GetContentTypeReaderActivator().Add(ListReader<bool>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Boolean" });
		GetContentTypeReaderActivator().Add(ListReader<char>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		GetContentTypeReaderActivator().Add(ListReader<uint8_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Byte" });
		GetContentTypeReaderActivator().Add(ListReader<float>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Float" });
		GetContentTypeReaderActivator().Add(ListReader<double>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Single" });
		GetContentTypeReaderActivator().Add(ListReader<char32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Char" });
		GetContentTypeReaderActivator().Add(ListReader<int16_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int16" });
		GetContentTypeReaderActivator().Add(ListReader<int32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int32" });
		GetContentTypeReaderActivator().Add(ListReader<int64_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.Int64" });
		GetContentTypeReaderActivator().Add(ListReader<uint16_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt16" });
		GetContentTypeReaderActivator().Add(ListReader<uint32_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt32" });
		GetContentTypeReaderActivator().Add(ListReader<uint64_t>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[System.UInt64" });
		GetContentTypeReaderActivator().Add(ListReader<Rectangle>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Rectangle" });
		GetContentTypeReaderActivator().Add(ListReader<Vector2>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector2" });
		GetContentTypeReaderActivator().Add(ListReader<Vector3>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector3" });
		GetContentTypeReaderActivator().Add(ListReader<Vector4>(), { "Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector4" });
	};
}