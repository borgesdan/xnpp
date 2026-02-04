#ifndef XNA_CONTENTPIPELINE_GRAPHICS_PIXELBITMAPCONTENT_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_PIXELBITMAPCONTENT_HPP

#include "BitmapContent.hpp"
#include <vector>
#include <Xna/Framework/Color.hpp>
#include <Xna/Framework/Vector4.hpp>

namespace Xna {
	template <class T>
	struct PixelBitmapContent : public BitmapContent {
		~PixelBitmapContent() override = default;			

		inline PixelBitmapContent() = default;
		inline PixelBitmapContent(size_t width, size_t height) : BitmapContent(width, height) {
			auto size = (static_cast<size_t>(width) * height);
			pixelData.resize(size);
		}

		void SetPixelData(const uint8_t* sourceData, size_t length) override {
			const auto data = reinterpret_cast<const T*>(sourceData);
			pixelData.assign(data, data + length);
		}

		bool TryGetFormat(SurfaceFormat& format) override {
			if constexpr (std::same_as<std::remove_cvref_t<T>, Color>) {
				format = SurfaceFormat::Color;
				return true;
			}
			if constexpr (std::same_as<std::remove_cvref_t<T>, float>) {
				format = SurfaceFormat::Single;
				return true;
			}
			if constexpr (std::same_as<std::remove_cvref_t<T>, Vector2>) {
				format = SurfaceFormat::Vector2;
				return true;
			}			
			if constexpr (std::same_as<std::remove_cvref_t<T>, Vector4>) {
				format = SurfaceFormat::Vector4;
				return true;
			}
			//if constexpr (std::same_as<std::remove_cvref_t<T>, HalfSingle>)
			//	format = SurfaceFormat::HalfSingle;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, HalfVector2>)
			//	format = SurfaceFormat::HalfVector2;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, HalfVector4>)
			//	format = SurfaceFormat::HalfVector4;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Bgra5551>)
			//	format = SurfaceFormat::Bgra5551;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Bgr565>)
			//	format = SurfaceFormat::Bgr565;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Bgra4444>)
			//	format = SurfaceFormat::Bgra4444;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Rg32>)
			//	format = SurfaceFormat::Rg32;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Rgba64>)
			//	format = SurfaceFormat::Rgba64;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Rgba1010102>)
			//	format = SurfaceFormat::Rgba1010102;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Alpha8>)
			//	format = SurfaceFormat::Alpha8;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, NormalizedByte2>)
			//	format = SurfaceFormat::NormalizedByte2;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, NormalizedByte4>)
			//	format = SurfaceFormat::NormalizedByte4;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, NormalizedShort2>)
			//	format = SurfaceFormat::NormalizedShort2;
			//if constexpr (std::same_as<std::remove_cvref_t<T>, NormalizedShort4>)
			//	format = SurfaceFormat::NormalizedShort4; //?
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Byte4>)
			//	format = SurfaceFormat::NormalizedShort4; //?
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Short4>)
			//	format = SurfaceFormat::NormalizedShort4; //?
			//if constexpr (std::same_as<std::remove_cvref_t<T>, Short2>)
			//	format = SurfaceFormat::NormalizedShort4; //?		
			return false;
		}

		std::vector<uint8_t> GetPixelData() const override {
			if constexpr (std::same_as<std::remove_cvref_t<T>, Vector4>) {
				constexpr size_t pixelSize = sizeof(Vector4); //sizeof(float) * 4

				const size_t rowSize = pixelSize * Width;
				std::vector<uint8_t> buffer(rowSize * Height);

				for (size_t y = 0; y < Height; ++y) {
					const Vector4* row = &pixelData[y * Width];
					std::memcpy(
						buffer.data() + y * rowSize,
						row,
						rowSize
					);
				}

				return buffer;
			}				
			else {
				//Color pixelSize = 4
				constexpr size_t pixelSize = sizeof(T); // Color = RGBA32

				const auto rowSize = pixelSize * Width;
				std::vector<uint8_t> buffer(rowSize * Height);

				for (size_t y = 0; y < Height; ++y) {
					const T* row = &pixelData[y * Width];
					uint8_t* dst = buffer.data() + y * rowSize;

					for (size_t x = 0; x < Width; ++x) {
						const uint32_t packed = row[x].PackedValue();

						dst[x * pixelSize + 0] = static_cast<uint8_t>(packed & 0xFF);
						dst[x * pixelSize + 1] = static_cast<uint8_t>((packed >> 8) & 0xFF);
						dst[x * pixelSize + 2] = static_cast<uint8_t>((packed >> 16) & 0xFF);
						dst[x * pixelSize + 3] = static_cast<uint8_t>((packed >> 24) & 0xFF);
					}
				}

				return buffer;
			}			
		}

		void SetPixel(size_t x, size_t y, T value)
		{
			if (x >= Width)
				throw std::runtime_error("x >= Width");
			if (y >= Height)
				throw std::runtime_error("y >= Height");
			At(x,y) = value;
		}

		T GetPixel(size_t x, size_t y) const {
			if (x >= Width)
				throw std::runtime_error("x >= Width");
			if (y >= Height)
				throw std::runtime_error("y >= Height");

			return At(x, y);
		}

		// Função Blit para copiar um bitmap para dentro deste
		void Blit(PixelBitmapContent<T> const& source, size_t destX, size_t destY) {
			for (size_t y = 0; y < source.Height; ++y) {
				// Calcula o início da linha no destino e na origem
				const auto destRowStart = (destY + y) * Width + destX;
				const auto srcRowStart = y * source.Width;

				for (size_t x = 0; x < source.Width; ++x) {
					// Copia o pixel
					pixelData[destRowStart + x] = source.pixelData[srcRowStart + x];
				}
			}
		}

		void BlitFast(const PixelBitmapContent& source, int destX, int destY) {
			// Validação básica para evitar escrita fora da memória
			if (destX + source.Width > this->Width || destY + source.Height > this->Height) {
				return;
			}

			for (int y = 0; y < source.Height; ++y) {
				// Ponteiro para o início da linha no destino
				T* destPtr = &this->pixelData[(destY + y) * this->Width + destX];

				// Ponteiro para o início da linha na origem
				const T* srcPtr = &source.pixelData[y * source.Width];

				// Copia a linha inteira de pixels
				std::memcpy(destPtr, srcPtr, source.Width * sizeof(T));
			}
		}

		// Acesso equivalente a pixelData[y][x]
		inline T& At(size_t x, size_t y) {
			return pixelData[y * Width + x]; 
		}
		
		inline const T& At(size_t x, size_t y) const	{ 
			return pixelData[y * Width + x]; 
		}

		std::vector<T>& Source() { return pixelData; }

		void ReplaceColor(T const& originalColor, T const& newColor) {
			for(auto& objArray : pixelData)	{
				if (objArray == originalColor)
					objArray = newColor;
			}
		}

		T* GetRow(size_t y) {
			return &pixelData[y * Width];
		}

		size_t RowPitch() const { return Width * sizeof(T); }

	private:
		std::vector<T> pixelData;
	};
}

#endif