#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECT_SHARED_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECT_SHARED_HPP

#include <variant>
#include <cstdint>
#include <string>
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Vector4.hpp"

namespace Xna {
    enum class EffectParameterClass
    {
        Scalar,
        Vector,
        Matrix,
        Object,
        Struct,
    };

    enum class EffectParameterType
    {
        Void,
        Bool,
        Int32,
        Single,
        String,
        Texture,
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
    };

    using EffectParameterTypeVariant
        = std::variant<bool, int32_t, float, std::string, Vector2, Vector3, Vector4, Matrix>;

    enum class EffectDirtyFlags
    {
        WorldViewProj = 1,
        World = 2,
        EyePosition = 4,
        MaterialColor = 8,
        Fog = 16, // 0x00000010
        FogEnable = 32, // 0x00000020
        AlphaTest = 64, // 0x00000040
        ShaderIndex = 128, // 0x00000080
        All = -1, // 0xFFFFFFFF
    };
}

#endif