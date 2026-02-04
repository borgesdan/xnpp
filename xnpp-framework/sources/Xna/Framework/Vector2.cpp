#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Quaternion.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
    void Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationArrayLength < sourceArrayLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = (source.X * matrix.M11 + source.Y * matrix.M21) + matrix.M41;
            destinationArray[index].Y = (source.X * matrix.M12 + source.Y * matrix.M22) + matrix.M42;
        }
    }

    void Vector2::Transform(std::vector<Vector2> sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    void Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
        Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceArrayLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationArrayLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t i = 0; i < length; ++i) {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex + i].X = (source.X * matrix.M11 + source.Y * matrix.M21) + matrix.M41;
            destinationArray[destinationIndex + i].Y = (source.X * matrix.M12 + source.Y * matrix.M22) + matrix.M42;
        }
    }

    void Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    void Vector2::TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationArrayLength < sourceArrayLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < sourceArrayLength; ++index) {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22;
        }
    }

    void Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    void Vector2::TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceArrayLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationArrayLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = (source.X * matrix.M11 + source.Y * matrix.M21);
            destinationArray[destinationIndex].Y = (source.X * matrix.M12 + source.Y * matrix.M22);
        }
    }

    void Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    void Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationArrayLength < sourceArrayLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        const auto rx = rotation.X + rotation.X;
        const auto ry = rotation.Y + rotation.Y;
        const auto rz = rotation.Z + rotation.Z;
        const auto rwz = rotation.W * rz;
        const auto rxx = rotation.X * rx;
        const auto rxy = rotation.X * ry;
        const auto ryy = rotation.Y * ry;
        const auto rzz = rotation.Z * rz;
        const auto a = 1.0f - ryy - rzz;
        const auto b = rxy - rwz;
        const auto c = rxy + rwz;
        const auto d = 1.0f - rxx - rzz;

        for (size_t index = 0; index < sourceArrayLength; ++index) {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * a + source.Y * b;
            destinationArray[index].Y = source.X * c + source.Y * d;
        }
    }

    void Vector2::Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), rotation, destinationArray.data(), destinationArray.size());
    }

    void Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
        Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceArrayLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationArrayLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        const auto rx = rotation.X + rotation.X;
        const auto ry = rotation.Y + rotation.Y;
        const auto rz = rotation.Z + rotation.Z;
        const auto rwz = rotation.W * rz;
        const auto rxx = rotation.X * rx;
        const auto rxy = rotation.X * ry;
        const auto ryy = rotation.Y * ry;
        const auto rzz = rotation.Z * rz;
        const auto a = 1.0f - ryy - rzz;
        const auto b = rxy - rwz;
        const auto c = rxy + rwz;
        const auto d = 1.0f - rxx - rzz;

        for (size_t i = 0; i < length; ++i) {
            const auto& source = sourceArray[sourceIndex = i];

            destinationArray[destinationIndex].X = source.X * a + source.Y * b;
            destinationArray[destinationIndex].Y = source.X * c + source.Y * d;
            ++sourceIndex;
            ++destinationIndex;
        }
    }

    void Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, rotation, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }
}