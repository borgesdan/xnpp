#ifndef XNA_FRAMEWORK_GRAPHICS_TEXTURE2D_HPP
#define XNA_FRAMEWORK_GRAPHICS_TEXTURE2D_HPP

#include "../Rectangle.hpp"
#include "../Color.hpp"
#include "Texture.hpp"
#include <vector>
#include <string>
#include <optional>
#include "Xna/CSharp/IO/Stream.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Represents a 2D grid of texels. 
	class Texture2D final : public Texture {
	public:	
		//Creates a new instance of this object.
		XNPP_API Texture2D(GraphicsDevice const& device, size_t width, size_t height);
		//Creates a new instance of this object.
		XNPP_API Texture2D(GraphicsDevice const& device, size_t width, size_t height, bool mipMap, SurfaceFormat format);

		virtual ~Texture2D() override {}

		//Gets the size of this resource.
		inline Rectangle Bounds() const { return Rectangle(0, 0, static_cast<int32_t>(impl->width), static_cast<int32_t>(impl->height)); }
		//Gets the format of the texture data.
		SurfaceFormat Format() const override { return impl->format; }
		//Gets the height of this texture resource, in pixels.
		size_t Height() const { return impl->height; }
		//Gets the number of texture levels in a multilevel texture. 
		XNPP_API int32_t LevelCount() const override;
		//Gets the width of this texture resource, in pixels.
		size_t Width() const{ return impl->width; }

		//Gets a copy of 2D texture data.
		template <typename T>
		void GetData(std::vector<T>& data) { GetData(data, 0, data.size()); }
		//Gets a copy of 2D texture data, specifying a start index and number of elements.
		template <typename T>
		void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount) { GetData(0, std::nullopt, data, startIndex, elementCount); }
		//Gets a copy of 2D texture data, specifying a mimap level, source rectangle, start index, and number of elements.
		template <typename T>
		void GetData(size_t level, std::optional<Rectangle> const& rect, std::vector<T>& data, size_t startIndex, size_t elementCount) {
			size_t sizeOfT = sizeof(T);
			auto ptr = reinterpret_cast<void*>(data.data());
			Platform::Texture2D_GetData(*this, level, rect, ptr, startIndex, elementCount, sizeOfT);
		}

		//Sets data to the texture.
		template <typename T>
		void SetData(std::vector<T> const& data) { SetData(data, 0, data.size()); }
		//Sets data to the texture.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount) { SetData(0, std::nullopt, data, startIndex, elementCount); }
		//Sets 2D texture data, specifying a mipmap level, source rectangle, start index, and number of elements.
		template <typename T>
		void SetData(size_t level, std::optional<Rectangle> const& rect, std::vector<T> const& data, size_t startIndex, size_t elementCount) {
			//size_t sizeOfT = sizeof(T);
			size_t sizeOfT = sizeof(Color);
			auto ptr = reinterpret_cast<const void*>(data.data());
			Platform::Texture2D_SetData(*this, level, rect, ptr, startIndex, elementCount, impl->hasMipmaps, sizeOfT);
		}

		//Saves texture data as a .jpg.
		XNPP_API void SaveAsJpeg(CSharp::Stream& stream, size_t width, size_t height);
		//Saves texture data as a .png.
		XNPP_API void SaveAsPng(CSharp::Stream& stream, size_t width, size_t height);

		//Loads texture data from a stream. 
		XNPP_API static Texture2D FromStream(GraphicsDevice const& device, CSharp::Stream& stream) { return FromStream(device, stream, 0, 0, false); }
		//Loads texture data from a data. 
		XNPP_API static Texture2D FromStream(GraphicsDevice const& device, CSharp::Stream& stream, size_t width, size_t height, bool zoom);

		Texture2D() { impl = std::make_shared<Implementation>(); }

		inline Texture2D(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(Texture2D const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		static inline constexpr size_t MaxTextureSize = 4096;

	private:
		struct Implementation {
			bool hasMipmaps{ false };
			size_t width{ 0 };
			size_t height{ 0 };			
			SurfaceFormat format{};
			PlatformImpl::Texture2DImpl platformImpl;
		};
	
		std::shared_ptr<Implementation> impl;	

		friend struct Platform;
	};
}

#endif