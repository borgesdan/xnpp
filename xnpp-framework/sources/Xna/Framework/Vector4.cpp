#include "Xna/Framework/Vector4.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Quaternion.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
    void Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationLength < sourceLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < sourceLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31 + source.W * matrix.M41;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32 + source.W * matrix.M42;
            destinationArray[index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33 + source.W * matrix.M43;
            destinationArray[index].W = source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34 + source.W * matrix.M44;
        }
    }

    void Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceLength, Matrix const& matrix, std::vector<Vector4>& destinationArray) {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    void Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31 + source.W * matrix.M41;
            destinationArray[destinationIndex].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32 + source.W * matrix.M42;
            destinationArray[destinationIndex].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33 + source.W * matrix.M43;
            destinationArray[destinationIndex].W = source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34 + source.W * matrix.M44;
        }
    }

    void Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length) {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    void Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationLength < sourceLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t index = 0; index < sourceLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.Z * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[index].Y = source.Z * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[index].Z = source.Z * num19 + source.Y * num20 + source.Z * num21;
            destinationArray[index].W = sourceArray[index].W;
        }
    }

    void Vector4::Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray) {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), rotation, destinationArray.data(), destinationArray.size());
    }

    void Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = source.X * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[destinationIndex].Y = source.X * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[destinationIndex].Z = source.X * num19 + source.Y * num20 + source.Z * num21;
            destinationArray[destinationIndex].W = source.W;
        }
    }

    void Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length) {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), sourceIndex, rotation, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }
}