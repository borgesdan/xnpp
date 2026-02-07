#include "BasicReaders.hpp"
#include "../../Graphics/GraphicsContentHelper.hpp"

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
		return SoundEffect(format, data, loopStart, loopLength, CSharp::TimeSpan::FromMilliseconds(static_cast<double>(num)));
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
			auto data = input.ReadByteBuffer(num2);
			texture2D.SetData(level, std::nullopt, *data, 0, num2);
		}

		return texture2D;
	}
}