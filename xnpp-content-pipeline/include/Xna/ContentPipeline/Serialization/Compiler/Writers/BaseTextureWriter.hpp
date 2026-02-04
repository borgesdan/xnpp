#ifndef XNA_WRITERS_BASETEXTUREWRITER_HPP
#define XNA_WRITERS_BASETEXTUREWRITER_HPP

#include "../BuiltinTypeWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"
#include "Xna/ContentPipeline/Graphics/BitmapContent.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"
#include <Xna/Framework/Graphics/Shared.hpp>
#include <Xna/CSharp/Exception.hpp>
#include <vector>
#include <cstdint>

namespace Xna {
	template <typename T, typename TRuntime>
	struct BaseTextureWriter : public BuiltinTypeWriter<T>{
		~BaseTextureWriter() override = default;

	protected:
		void WriteT(ContentWriter& output, const T* value) override {
			//value.Validate(new GraphicsProfile ? (output.TargetProfile));
			auto bitmapContent = value->Faces()[0][0];
			SurfaceFormat format{SurfaceFormat::Color};
			
			if (!bitmapContent->TryGetFormat(format))
				throw CSharp::InvalidOperationException("Surface format not founded.");

			WriteTextureHeader(output, format, bitmapContent->Width, bitmapContent->Height, value->Faces().Count(), value->Faces()[0].Count());
			WriteTextureData(output, value);
		}

		virtual void WriteTextureHeader(ContentWriter& output, SurfaceFormat format,
			size_t width, size_t height, size_t depth, size_t mipLevels) = 0;

		virtual void WriteTextureData(ContentWriter& output, const T* texture) {
			for(const auto& face : texture->Faces()) {
				for(const auto& bitmap : face) {
					const auto pixelData = bitmap->GetPixelData();
					ContentWriter::Base& output1 = output;

					output1.Write(static_cast<int32_t>(pixelData.size()));
					WriteBits(output, *bitmap, pixelData);
				}
			}
		}

		static void WriteBits(ContentWriter& output, BitmapContent const& bitmap, std::vector<uint8_t> const& pixelData) {
			ContentWriter::Base& output1 = output;

			switch (output.TargetPlatform())
			{
			case TargetPlatform::Windows:
			case TargetPlatform::WindowsPhone:				
				output1.Write(pixelData.data(), pixelData.size());
				break;
			case TargetPlatform::Xbox360:
				//BaseTextureWriter<T, TRuntime>.WriteXboxBits(output, bitmap, pixelData);
				throw CSharp::NotSupportedException();
				break;
			default:
				throw CSharp::NotSupportedException();
			}
		}
	};
}

#endif