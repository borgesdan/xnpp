#ifndef XNA_CONTENTPIPELINE_EXTERNALREFERENCE_HPP
#define XNA_CONTENTPIPELINE_EXTERNALREFERENCE_HPP

#include "ContentItem.hpp"
#include "PathUtils.hpp"
#include "ContentIdentity.hpp"
#include <filesystem>
#include <optional>

namespace Xna {
	struct ExternalReferenceBase : public ContentItem {
	public:
		virtual ~ExternalReferenceBase() = default;
		inline std::filesystem::path Filename() const { return filename; }

		void Replace(std::filesystem::path const& filename, std::optional<ContentIdentity> const& relativeToContent) {
			if (relativeToContent)
				this->filename = PathUtils::GetFullPath(relativeToContent->SourceFileName / filename);
			else
				this->filename = PathUtils::GetFullPath(filename);
		}

	protected:
		inline ExternalReferenceBase() = default;

		inline ExternalReferenceBase(std::filesystem::path const& filename) {
			this->filename = PathUtils::GetFullPath(filename);
		}

		inline ExternalReferenceBase(std::filesystem::path const& filename, ContentIdentity const& relativeToContent) {
			this->filename = PathUtils::GetFullPath(relativeToContent.SourceFileName / filename);
		}		

		std::filesystem::path filename;
	};

	template <typename T>
	struct ExternalReference : public ExternalReferenceBase {
		inline ExternalReference() : ExternalReferenceBase() {}
		inline ExternalReference(std::filesystem::path const& filename) : ExternalReferenceBase(filename) {}		
		inline ExternalReference(std::filesystem::path const& filename, ContentIdentity const& relativeToContent) : ExternalReferenceBase(filename, relativeToContent){}		
	};
}

#endif