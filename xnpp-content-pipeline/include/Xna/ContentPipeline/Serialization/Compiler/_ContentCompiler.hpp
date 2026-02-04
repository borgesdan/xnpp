#ifndef XNA_COMPILER__CONTENTCOMPILER_HPP
#define XNA_COMPILER__CONTENTCOMPILER_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <stack>
#include <any>
#include <string>
#include <filesystem>
#include <Xna/CSharp/Type.hpp>
#include <Xna/CSharp/IO/Stream.hpp>
#include "ContentTypeWriter.hpp"
#include <Xna/Framework/Content/Pipeline/Shared.hpp>
#include <Xna/Framework/Graphics/Shared.hpp>

namespace Xna {
	class ContentCompiler {
	public:		
		std::shared_ptr<ContentTypeWriter> GetTypeWriter(CSharp::Type type);

		inline ContentCompiler(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ContentCompiler const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	private:
		ContentCompiler();
		void Compile(
			std::shared_ptr<CSharp::Stream> const& output,
			std::any const& value,
			TargetPlatform targetPlatform,
			GraphicsProfile targetProfile,
			bool compressContent,
			std::filesystem::path const& rootDirectory,
			std::filesystem::path const& referenceRelocationPath);

		bool ShouldCompressContent(TargetPlatform targetPlatform, std::any const& value);
		std::shared_ptr<ContentTypeWriter> GetTypeWriter(CSharp::Type const& type, std::vector<CSharp::Type>& dependencies);
		std::shared_ptr<ContentTypeWriter> GetTypeWriterInternal(CSharp::Type const& type);
		void AddTypeWriter(std::shared_ptr<ContentTypeWriter> const& writer);
		void InitializeTypeWriter(std::shared_ptr<ContentTypeWriter>& writer);
		void RecordDependency(CSharp::Type const& type);

	private:
		struct Implementation;;

		std::shared_ptr<Implementation> impl;

		friend class BuildCoordinator;
		friend class ContentWriter;
	};
}

#endif