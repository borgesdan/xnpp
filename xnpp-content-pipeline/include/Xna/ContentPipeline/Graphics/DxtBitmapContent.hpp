#ifndef XNA_GRAPHICS_DXTBITMAPCONTENT_HPP
#define XNA_GRAPHICS_DXTBITMAPCONTENT_HPP

#include "BitmapContent.hpp"
#include <Xna/Framework/Graphics/Shared.hpp>
#include <vector>
#include <cstdint>
#include <exception>

namespace Xna{
	struct DxtBitmapContent : public BitmapContent {
		using Base = BitmapContent;
		~DxtBitmapContent() override = default;

		inline void SetPixelData(const uint8_t* sourceData, size_t length) override {
			if (sourceData == nullptr)
				throw std::invalid_argument("sourceData");
			if (length != pixelData.size())
				throw std::invalid_argument("length");

			pixelData.assign(sourceData, sourceData + length);
		}		

		std::vector<uint8_t> GetPixelData() const override {
			return pixelData;
		}

	protected:
		inline DxtBitmapContent(size_t blockSize) 
			: blockSize(blockSize){}
		inline DxtBitmapContent(size_t blockSize, size_t width, size_t height)
			: blockSize(blockSize), Base(width, height){
			pixelData.resize(PixelDataSize(blockSize, width, height));
		}	

	private:
		static size_t PixelDataSize(size_t blockSize, size_t width, size_t height) {
			width = width + 3 >> 2;
			height = height + 3 >> 2;
			return width * height * blockSize;
		}

		std::vector<uint8_t> pixelData;
		size_t blockSize{0};
	};

	struct Dxt3BitmapContent : public DxtBitmapContent {
		using Base = DxtBitmapContent;
		~Dxt3BitmapContent() override = default;

		inline Dxt3BitmapContent() 
			: Base(BlockSize){}

		inline Dxt3BitmapContent(size_t width, size_t height) 
			: Base(BlockSize, width, height){}
		
		inline bool TryGetFormat(SurfaceFormat& format) override {
			format = GpuFormat;
			return true;
		}

	private:
		static constexpr size_t BlockSize = 16;
		static constexpr SurfaceFormat GpuFormat = SurfaceFormat::Dxt3;
	};
}

#endif