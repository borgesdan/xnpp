#ifndef XNA_CONTENTPIPELINE_BUILDITEM_HPP
#define XNA_CONTENTPIPELINE_BUILDITEM_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <filesystem>
#include <memory>
#include "BuildRequest.hpp"
#include "CustomComparerCollection.hpp"

namespace Xna {
	struct BuildItem final {
		inline BuildItem() { impl = std::make_shared<Implementation>(); }

		inline Xna::BuildRequest& BuildRequest() const { return impl->BuildRequest; }
		inline std::filesystem::path& OutputFilename() const { return impl->OutputFilename; }
		inline std::vector<std::filesystem::path>& ExtraOutputFiles() const { return impl->ExtraOutputFiles; }
		inline TimePoint& SourceTimestamp() const { return impl->SourceTimestamp; }
		inline std::unordered_map<std::filesystem::path, TimePoint>& Dependencies() const { return impl->Dependecies; }
		inline CustomComparerCollection<std::filesystem::path>& Requests() const { return impl->Requests; }
		inline bool& IsBuilt() const { return impl->IsBuilt; }
		inline bool& IsWanted() const { return impl->IsWanted; }
		inline std::optional<bool>& NeedsRebuild() const { return impl->NeedsRebuild; }
		inline std::string& RebuildReason() const { return impl->RebuildReason; }
		

		inline void SetNeedsRebuild(std::string const& reason) {
			impl->NeedsRebuild = true;
			impl->RebuildReason = reason;
		}

		inline void ClearNeedsRebuild() {
			impl->NeedsRebuild = false;
		}

		inline BuildItem(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BuildItem const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			//[ContentSerializer(FlattenContent = true)]
			Xna::BuildRequest BuildRequest;
			//[ContentSerializer(ElementName = "Output", AllowNull = false)]
			std::filesystem::path OutputFilename;
			//[ContentSerializer(CollectionItemName = "Extra", AllowNull = false, FlattenContent = true)]
			std::vector<std::filesystem::path> ExtraOutputFiles;
			//[ContentSerializer(ElementName = "Time")]
			TimePoint SourceTimestamp;
			//[ContentSerializer(CollectionItemName = "Dependency", FlattenContent = true)]
			std::unordered_map<std::filesystem::path, TimePoint> Dependecies;
			//[ContentSerializer(CollectionItemName = "Request", FlattenContent = true)]
			//= new CustomComparerCollection<string>((IEqualityComparer<string>)StringComparer.OrdinalIgnoreCase);
			CustomComparerCollection<std::filesystem::path> Requests;
			//[ContentSerializerIgnore]
			bool IsBuilt{ false };
			//[ContentSerializerIgnore]
			bool IsWanted{ false };

			std::optional<bool> NeedsRebuild;
			std::string RebuildReason;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif