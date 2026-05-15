#ifndef XNA_CONTENTPIPELINE_BUILDITEMCOLLECTION_HPP
#define XNA_CONTENTPIPELINE_BUILDITEMCOLLECTION_HPP

#include "BuildCoordinatorSettings.hpp"
#include "BuildItem.hpp"
#include "TimestampCache.hpp"
#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <Xna/CSharp/Collection.hpp>
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	class BuildItemCollection final : public CSharp::Collection<BuildItem> {
	public:
		BuildItemCollection() : Collection() { impl = std::make_shared<Implementation>(); }

		inline bool BuildSuccessful() const { return impl->buildSuccessful; }
		inline void BuildSuccessful(bool value) { impl->buildSuccessful = value; }	

		XNPP_API BuildItem Find(BuildRequest const& request);
		XNPP_API BuildItem Find(std::filesystem::path const& outputFilename);
		XNPP_API void WantItem(BuildItem const& item);
		XNPP_API BuildItem PopWantedItem();
		inline size_t WantedItemsCount() const { return impl->wantedItems.size(); }
		XNPP_API void ReverseWantedItems(size_t fromIndex);
		XNPP_API bool RemoveUnwantedItems();
		XNPP_API void Save(std::filesystem::path const& filename);
		XNPP_API static BuildItemCollection Load(std::filesystem::path const& filename);
		XNPP_API static BuildItemCollection Load(std::filesystem::path const& filename, std::string& rebuildReason);
		XNPP_API bool UpdateSettings(BuildCoordinatorSettings const& newSettings, std::string& rebuildReason);
		XNPP_API bool UpdateAssemblies(TimestampCache& timestampCache, std::string& rebuildReason);		
		XNPP_API void GetAssemblyTimestamps(std::unordered_map<std::filesystem::path, TimePoint>& timestamps);
		XNPP_API void DeleteStaleOutputFiles();
		XNPP_API std::vector<std::pair<std::filesystem::path, std::string>> GetDuplicateWarnings();

		inline BuildItemCollection(std::nullptr_t) : Collection(nullptr ){ impl = nullptr; }
		inline bool operator==(BuildItemCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		XNPP_API void InsertItem(size_t index, BuildItem const& item) override;
		XNPP_API void RemoveItem(size_t index) override;
		XNPP_API void ClearItems() override;
		XNPP_API void SetItem(size_t index, BuildItem const& item) override;

	private:
		XNPP_API std::string GetDuplicateWarning(std::vector<BuildItem> const& outputs);
		XNPP_API std::string GetDuplicateWarningReferees(BuildItem const& output);
	private:
		struct Implementation {
			bool buildSuccessful;
			std::unordered_map<BuildRequest, BuildItem> requestTable;
			std::unordered_map<std::filesystem::path, BuildItem> outputFilenameTable;
			std::vector<BuildItem> wantedItems;
			std::vector<std::filesystem::path> previousOutputs;
			// [ContentSerializer(ElementName = "Settings")]
			BuildCoordinatorSettings buildSettings;
			// [ContentSerializer(ElementName = "Assemblies", CollectionItemName = "Assembly")]
			std::unordered_map<std::string, TimePoint> assemblyTimestamps;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif