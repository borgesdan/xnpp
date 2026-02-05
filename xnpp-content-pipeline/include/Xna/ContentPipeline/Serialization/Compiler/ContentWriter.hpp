#ifndef XNA_COMPILER_CONTENTWRITER_HPP
#define XNA_COMPILER_CONTENTWRITER_HPP

#include <any>
#include <queue>
#include <unordered_map>
#include "_ContentWriter.hpp"
#include <Xna/CSharp/IO/MemoryStream.hpp>
#include <Xna/CSharp/Exception.hpp>
#include <Xna/Internal/Misc.hpp>
#include "_ContentCompiler.hpp"
#include "ContentTypeWriter.hpp"
#include "Xna/ContentPipeline/ExternalReference.hpp"
#include "Xna/ContentPipeline/PathUtils.hpp"

namespace Xna {
	struct ContentWriter::Implementation {
		ContentCompiler compiler{ nullptr };
		std::shared_ptr<CSharp::Stream> finalOutput;
		std::shared_ptr<CSharp::MemoryStream> headerData{ nullptr };
		std::shared_ptr<CSharp::MemoryStream> contentData{ nullptr };
		std::vector<std::shared_ptr<ContentTypeWriter>> typeWriters;
		Xna::TargetPlatform targetPlatform{ TargetPlatform::Windows };
		GraphicsProfile targetProfile{ GraphicsProfile::HiDef };
		bool compressContent{ false };
		std::filesystem::path rootDirectory;
		std::filesystem::path referenceRelocationPath;
		
		std::unordered_map<const void *, int32_t> sharedResourceNames;
		std::unordered_map<CSharp::Type, int32_t> typeTable;
		std::unordered_map<const void*, bool> recurseDetector;
		std::queue<const void*> sharedResources;
	};

	template <typename T> void ContentWriter::WriteExternalReference(const ExternalReference<T>* reference) {
		if (reference == nullptr || reference->Filename().empty()) {
			BinaryWriter::Write(std::string(""));
			return;
		}
		auto filename1 = reference->Filename();

		//string filename2 = filename1.EndsWith(".xnb", StringComparison.OrdinalIgnoreCase) ? filename1.Substring(0, filename1.Length - ".xnb".Length) : throw new ArgumentException();
		if (!filename1.has_extension() || filename1.extension() != ".xnb")
			throw CSharp::ArgumentException("filename must end with .xnb");

		const auto& filename2 = filename1.replace_extension();

		//if (!filename2.StartsWith(this.rootDirectory, StringComparison.OrdinalIgnoreCase)) throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.WriteExternalReferenceBadPath, (object)filename2, (object)this.rootDirectory));
		//TODO: [!] não implementato

		//this.Write(PathUtils.GetRelativePath(PathUtils.GetAbsoluteUri(this.referenceRelocationPath), filename2));
		BinaryWriter::Write(PathUtils::GetRelativePath(PathUtils::GetAbsoluteUri(impl->referenceRelocationPath), filename2).string());
	}	

	template <typename T> void ContentWriter::WriteSharedResource(const T* value) {
		if (value == nullptr) {
			Write7BitEncodedInt(0);
		}
		else
		{			
			size_t num{0};			
			auto contains = impl->sharedResourceNames.contains(value);

			if (!contains)
			{
				num = impl->sharedResourceNames.size() + 1;
				impl->sharedResourceNames[value] = static_cast<int32_t>(num);
				
				impl->sharedResources.push(value);
			} else {
				num = impl->sharedResourceNames[value];
			}			

			Write7BitEncodedInt(static_cast<int32_t>(num));
		}
	}

	template <typename T> void ContentWriter::InvokeWriter(const T* value, ContentTypeWriter& writer) {
		auto contentTypeWriter = dynamic_cast<ContentTypeWriterT<T>*>(&writer);

		if (contentTypeWriter != nullptr) 
			contentTypeWriter->WriteT(*this, value);
		else {
			writer.Write(*this, *value);
		}
;	}

	template <typename T> void ContentWriter::InvokeWriter(std::any const& value, ContentTypeWriter& writer) {
		auto contentTypeWriter = dynamic_cast<ContentTypeWriterT<T>*>(&writer);

		if (contentTypeWriter != nullptr) {
			auto value1 = std::any_cast<T>(value);
			contentTypeWriter->WriteT(*this, &value1);
		}			
		else
			writer.Write(*this, value);
	}

	template <typename T> void ContentWriter::WriteRawObject(const T* value, ContentTypeWriter& typeWriter) {
		if (value == nullptr)
			throw CSharp::ArgumentNullException("value");

		InvokeWriter(value, typeWriter);
	}

	template <typename T> void ContentWriter::WriteRawObject(const T* value) {
		if (value == nullptr)
			throw CSharp::ArgumentNullException("value");

		int32_t _{ 0 };
		auto typeWriter = GetTypeWriter(CSharp::Type(typeid(T)), _);
		InvokeWriter(value, *typeWriter);
	}

	template <typename T> void ContentWriter::WriteObject(const T* value, ContentTypeWriter& typeWriter) {
		if (typeWriter.TargetIsValueType()) //TODO: [!] verificar
			InvokeWriter(value, typeWriter);
		else
			WriteObject(value);
	}

	template <typename T> void ContentWriter::WriteObject(const T* value) {
		if (value == nullptr) {
			Write7BitEncodedInt(0);
			return;
		}

		int32_t typeIndex{0};	
		std::shared_ptr<ContentTypeWriter> typeWriter;

		if constexpr (std::same_as<std::remove_cvref_t<T>, std::any>) {
			typeWriter = GetTypeWriter(value->type(), typeIndex);
		}
		else {
			typeWriter = GetTypeWriter(CSharp::Type(typeid(T)), typeIndex);
		}	

		if (!typeWriter)
			throw std::runtime_error("TypeWriter not founded.");

		Write7BitEncodedInt(typeIndex + 1);		
		
		if (impl->recurseDetector.contains(value))
			throw CSharp::InvalidOperationException();

		impl->recurseDetector[value] = true;
		InvokeWriter(value, *typeWriter);
		impl->recurseDetector.erase(value);
	}

	inline Xna::TargetPlatform ContentWriter::TargetPlatform() const {
		return impl->targetPlatform;
	}
}

#endif