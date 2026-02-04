#include "Xna/Framework/Vector3.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Framework/Matrix.hpp"

namespace Xna {
    void Vector3::Transform(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destinationLength < sourceArrayLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = (source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31) + matrix.M41;
            destinationArray[index].Y = (source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32) + matrix.M42;
            destinationArray[index].Z = (source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33) + matrix.M43;
        }
    }

    void Vector3::Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    void Vector3::Transform(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceArrayLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < length; ++index)
        {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = (source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31) + matrix.M41;
            destinationArray[destinationIndex + index].Y = (source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32) + matrix.M42;
            destinationArray[destinationIndex + index].Z = (source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33) + matrix.M43;
        }
    }

    void Vector3::Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    void Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destionationArrayLength)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (destionationArrayLength < sourceArrayLength)
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }
    }

    void Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    void Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (sourceArray == nullptr)
            throw CSharp::ArgumentNullException("sourceArray");
        if (destinationArray == nullptr)
            throw CSharp::ArgumentNullException("destinationArray");
        if (sourceArrayLength < sourceIndex + length)
            throw CSharp::ArgumentException("NotEnoughSourceSize");
        if (destinationLength < (destinationIndex + length))
            throw CSharp::ArgumentException("NotEnoughTargetSize");

        for (size_t index = 0; index < length; ++index) {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[destinationIndex + index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[destinationIndex + index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }
    }

    void Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

}