#ifndef XNA_FRAMEWORK_GRAPHICS_TEXTURE3D_HPP
#define XNA_FRAMEWORK_GRAPHICS_TEXTURE3D_HPP

#include <memory>
#include <vector>
#include "Texture.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Represents a 3D volume of texels.
	class Texture3D final : public Texture {
	public:
		//Initializes a new instance of the Texture3D class.
		Texture3D(
			GraphicsDevice& graphicsDevice,
			size_t width,
			size_t height,
			size_t depth,
			bool mipMap,
			SurfaceFormat format);

		~Texture3D() override = default;

		//Gets the depth of this volume texture resource, in pixels.
		inline size_t Depth() const { return impl->depth; }
		//Gets the height of this texture resource, in pixels.
		size_t Height() const { return impl->height; }
		//Gets the width of this texture resource, in pixels.
		size_t Width() const { return impl->width; }

		//Gets a copy of the texture data.
		template <class T> void GetData(std::vector<T>& data) { GetData(data, 0, data.size()); }
		//Gets a copy of the texture data, specifying the starting index and number of elements to copy.
		template <class T> void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount) {
			GetData(0, 0, 0, impl->width, impl->height, 0, impl->depth, data, startIndex, elementCount);
		}
		//Gets a copy of the texture data, specifying the level and dimensions of the volume texture to copy.
		template <class T> void GetData(size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back,
			std::vector<T>& data, size_t startIndex, size_t elementCount) {
			Platform::Texture3D_GetData(*this, level, left, top, right, bottom, front, back, reinterpret_cast<void*>(data.data()), startIndex, elementCount, sizeof(T));
		}

		//Gets a copy of the texture data.
		template <class T> void SetData(std::vector<T>& data) { SetData<T>(data, 0, data.size()); }
		//Gets a copy of the texture data, specifying the starting index and number of elements to copy.
		template <class T> void SetData(std::vector<T>& data, size_t startIndex, size_t elementCount) {
			SetData<T>(0, 0, 0, impl->width, impl->height, 0, impl->depth, data, 0, data.size());
		}
		//Gets a copy of the texture data, specifying the level and dimensions of the volume texture to copy.
		template <class T> void SetData(size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back, std::vector<T>& data, size_t startIndex, size_t elementCount) {
			Platform::Texture3D_SetData(*this, level, left, top, right, bottom, front, back, reinterpret_cast<void*>(data.data()), startIndex, elementCount);
		}

		inline Texture3D(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(Texture3D const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }		

	private:
		struct Implementation {
			size_t depth{ 0 };
			size_t height{ 0 };
			size_t width{ 0 };
			size_t mipLevels{ 1 };
			SurfaceFormat format{};
			bool hasMipMap{ false };
			PlatformImpl::Texture3DImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;	

		friend struct Platform;
	};
}

#endif