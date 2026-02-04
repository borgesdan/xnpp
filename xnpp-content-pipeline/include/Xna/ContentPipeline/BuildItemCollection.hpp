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

namespace Xna {
	class BuildItemCollection final : public CSharp::Collection<BuildItem> {
	public:
		BuildItemCollection() : Collection() { impl = std::make_shared<Implementation>(); }

		inline bool BuildSuccessful() const { return impl->buildSuccessful; }
		inline void BuildSuccessful(bool value) { impl->buildSuccessful = value; }	

		BuildItem Find(BuildRequest const& request);
		BuildItem Find(std::filesystem::path const& outputFilename);
		void WantItem(BuildItem const& item);
		BuildItem PopWantedItem();
		inline size_t WantedItemsCount() const { return impl->wantedItems.size(); }
		void ReverseWantedItems(size_t fromIndex);
		bool RemoveUnwantedItems();
		void Save(std::filesystem::path const& filename);
		static BuildItemCollection Load(std::filesystem::path const& filename);
		static BuildItemCollection Load(std::filesystem::path const& filename, std::string& rebuildReason);
		bool UpdateSettings(BuildCoordinatorSettings const& newSettings, std::string& rebuildReason);
		bool UpdateAssemblies(TimestampCache& timestampCache, std::string& rebuildReason);		
		void GetAssemblyTimestamps(std::unordered_map<std::filesystem::path, TimePoint>& timestamps);
		void DeleteStaleOutputFiles();
		std::vector<std::pair<std::filesystem::path, std::string>> GetDuplicateWarnings();

		inline BuildItemCollection(std::nullptr_t) : Collection(nullptr ){ impl = nullptr; }
		inline bool operator==(BuildItemCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		void InsertItem(size_t index, BuildItem const& item) override;
		void RemoveItem(size_t index) override;
		void ClearItems() override;
		void SetItem(size_t index, BuildItem const& item) override;

	private:
		std::string GetDuplicateWarning(std::vector<BuildItem> const& outputs);
		std::string GetDuplicateWarningReferees(BuildItem const& output);
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