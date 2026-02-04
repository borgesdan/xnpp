#ifndef XNA_COMPILER__CONTENTWRITER_HPP
#define XNA_COMPILER__CONTENTWRITER_HPP

#include <string>
#include <memory>
#include <cstdint>
#include <filesystem>
#include <Xna/CSharp/IO/BinaryWriter.hpp>
#include <Xna/CSharp/Type.hpp>
#include <Xna/Framework/Content/Pipeline/Shared.hpp>
#include <Xna/Framework/Graphics/Shared.hpp>
#include <Xna/Framework/Vector4.hpp>
#include <Xna/Framework/Matrix.hpp>
#include <Xna/Framework/Quaternion.hpp>
#include <Xna/Framework/Color.hpp>

namespace Xna {
	class ContentCompiler;
	class ContentTypeWriter;
	template <typename T> class ExternalReference;

	class ContentWriter : public CSharp::BinaryWriter {
	public:
		using Base = CSharp::BinaryWriter;

		//Remover
		ContentWriter() = default;

		ContentWriter(
			ContentCompiler compiler,
			std::shared_ptr<CSharp::Stream> output,
			TargetPlatform targetPlatform,
			GraphicsProfile targetProfile,
			bool compressContent,
			std::filesystem::path rootDirectory,
			std::filesystem::path referenceRelocationPath);
	
		template <typename T> void WriteObject(const T* value);
		template <typename T> void WriteObject(const T* value, ContentTypeWriter& typeWriter);
		template <typename T> void WriteRawObject(const T* value);
		template <typename T> void WriteRawObject(const T* value, ContentTypeWriter& typeWriter);
		template <typename T> void WriteSharedResource(const T* value);
		template <typename T> void WriteExternalReference(const ExternalReference<T>* reference);

		void Write(Vector2 const& value);
		void Write(Vector3 const& value);
		void Write(Vector4 const& value);
		void Write(Matrix const& value);
		void Write(Quaternion const& value);
		
		inline void Write(Color const& value) { BinaryWriter::Write(value.PackedValue()); }		

		inline ContentWriter(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ContentWriter const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }		

		Xna::TargetPlatform TargetPlatform() const;

	private:
		void FlushOutput();
		template <typename T> void InvokeWriter(const T* value, ContentTypeWriter& typeWriter);
		std::shared_ptr<ContentTypeWriter> GetTypeWriter(CSharp::Type const& type, int32_t& typeIndex);
		void WriteSharedResources();
		void WriteHeader();
		void WriteFinalOutput();
		void WriteUncompressedOutput();
		void WriteCompressedOutput();
		void WriteVersionNumber(uint16_t version);

		template <typename T> void InvokeWriter(std::any const& value, ContentTypeWriter& typeWriter);

	private:
		struct Implementation;

		std::shared_ptr<Implementation> impl;

		static constexpr uint16_t XnbVersion = 5;
		static constexpr uint16_t XnbCompressedVersion = 32773;
		static constexpr uint16_t XnbVersionProfileMask = 32512;
		static constexpr int32_t XnbVersionProfileShift = 8;
		static constexpr int32_t XnbVersionOffset = 4;
		static constexpr int32_t XnbFileSizeOffset = 6;
		static constexpr int32_t XnbPrologueSize = 10;	

		friend class ContentCompiler;
	};
}

#endif