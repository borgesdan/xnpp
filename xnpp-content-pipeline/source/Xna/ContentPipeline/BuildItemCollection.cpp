#include "Xna/ContentPipeline/BuildItemCollection.hpp"
#include "Xna/ContentPipeline/PathUtils.hpp"
#include <algorithm>
#include <format>
#include <Xna/Internal/Misc.hpp>

namespace Xna {
    struct Resources {
        static constexpr std::string_view RebuildBecauseCantLoadCache = "Rebuilding all content because {0} could not be read";
        static constexpr std::string_view RebuildBecauseSettingsChanged = "Rebuilding all content because build settings have changed";
        static constexpr std::string_view WarnDuplicateAsset = "Asset was built {0} times with different settings:";
        static constexpr std::string_view WarnDuplicateAssetNoProcessor = "no processor";
        static constexpr std::string_view WarnDuplicateAssetBuiltUsing = "using {0} and {1}";
        static constexpr std::string_view WarnDuplicateAssetReferencedBy = ", referenced by {0}";
    };

	void BuildItemCollection::InsertItem(size_t index, BuildItem const& item) {
        if (item == nullptr)
            throw std::invalid_argument("Item is null.");

        if (impl->requestTable.contains(item.BuildRequest()))
            throw std::invalid_argument("BuildItemCollection already contains the BuildItem");
        
        if (impl->outputFilenameTable.contains(item.OutputFilename()))
            throw std::invalid_argument("Build Item Collection already has an OutputFilename provided.");

        Collection::InsertItem(index, item);

        impl->requestTable[item.BuildRequest()] = item;
        impl->outputFilenameTable[item.OutputFilename()] = item;
	}

    void BuildItemCollection::RemoveItem(size_t index) {
        auto& buildItem = (*this)[index];
        Collection::RemoveItem(index);

        impl->requestTable.erase(buildItem.BuildRequest());
        impl->outputFilenameTable.erase(buildItem.OutputFilename());
        
        Xna::Misc::Erase(impl->wantedItems, buildItem);
    }

    void BuildItemCollection::ClearItems() {
        Collection::ClearItems();
        impl->requestTable.clear();
        impl->outputFilenameTable.clear();
        impl->wantedItems.clear();
    }

    void BuildItemCollection::SetItem(size_t index, BuildItem const& item) {
        throw std::runtime_error("BuildItemCollection::SetItem is not supported.");
    }

    BuildItem BuildItemCollection::Find(BuildRequest const& request) {
        if (impl->requestTable.contains(request))
            return impl->requestTable[request];

        return nullptr;
    }

    BuildItem BuildItemCollection::Find(std::filesystem::path const& outputFilename) {
        if (impl->outputFilenameTable.contains(outputFilename))
            return impl->outputFilenameTable[outputFilename];

        return nullptr;
    }

    void BuildItemCollection::WantItem(BuildItem const& item) {
        if (item.IsWanted())
            return;
        impl->wantedItems.push_back(item);
        item.IsWanted() = true;
    }

    BuildItem BuildItemCollection::PopWantedItem() {
        if (impl->wantedItems.size() <= 0)
            return nullptr;
        
        const auto index = impl->wantedItems.size() - 1;
        const auto wantedItem = impl->wantedItems[index];
        impl->wantedItems.erase(impl->wantedItems.begin() + index);
        return wantedItem;
    }

    void BuildItemCollection::ReverseWantedItems(size_t fromIndex) {
        if (fromIndex >= impl->wantedItems.size())
            return;

        std::reverse(impl->wantedItems.begin() + fromIndex, impl->wantedItems.end());
    }

    bool BuildItemCollection::RemoveUnwantedItems() {
        auto flag = false;
        size_t index = 0;
        while (index < Count())
        {
            if ((*this)[index].IsWanted()) {
                ++index;
            }
            else {
                RemoveAt(index);
                flag = true;
            }
        }
        return flag;
    }

    void BuildItemCollection::Save(std::filesystem::path const& filename) {
        //TODO: [!] use XML
        /*XmlWriterSettings settings = new XmlWriterSettings();
        settings.Indent = true;
        try
        {
            using (XmlWriter output = XmlWriter.Create(filename, settings))
                IntermediateSerializer.Serialize<BuildItemCollection>(output, this, filename);
        }
        catch (UnauthorizedAccessException ex)
        {
            throw new PipelineException(ex.Message, (Exception)ex);
        }*/

        throw std::runtime_error("BuildItemCollection::Save is not implemented.");
    }

    BuildItemCollection BuildItemCollection::Load(std::filesystem::path const& filename) {
        std::string rebuildReason;
        return Load(filename, rebuildReason);
    }

    BuildItemCollection BuildItemCollection::Load(std::filesystem::path const& filename, std::string& rebuildReason) {
        BuildItemCollection buildItemCollection{nullptr};
        try
        {
            if (std::filesystem::exists(filename)) {
                //TODO: [!] uso de xml
               /* using (XmlReader input = XmlReader.Create(filename))
                    buildItemCollection = IntermediateSerializer.Deserialize<BuildItemCollection>(input, filename);*/
                buildItemCollection.BuildSuccessful(false);
            }
            else
                buildItemCollection = BuildItemCollection();
        }
        catch(...)
        {
            buildItemCollection = BuildItemCollection();
            
            if (rebuildReason.empty())
                rebuildReason = std::format(Resources::RebuildBecauseCantLoadCache, filename.filename().string());
        }
        
        buildItemCollection.impl->previousOutputs.reserve(buildItemCollection.Count() * 2);

        for (size_t index = 0; index < buildItemCollection.Count(); ++index)
        {
            buildItemCollection.impl->previousOutputs.push_back(buildItemCollection[index].OutputFilename());

            for(size_t index1 = 0; index < buildItemCollection[index1].ExtraOutputFiles().size(); ++index1)
                buildItemCollection.impl->previousOutputs.push_back(buildItemCollection[index].ExtraOutputFiles()[index1]);
        }

        return buildItemCollection;
    }

    bool BuildItemCollection::UpdateSettings(BuildCoordinatorSettings const& newSettings, std::string& rebuildReason) {
        bool flag = false;
        if (!newSettings.MatchesPreviousSettings(impl->buildSettings)) {
            flag = true;
            if (rebuildReason.empty() && Count() > 0)
                rebuildReason = Resources::RebuildBecauseSettingsChanged;
        }
        
        impl->buildSettings = newSettings;

        return flag;
    }

    bool BuildItemCollection::UpdateAssemblies(TimestampCache& timestampCache, std::string& rebuildReason) {
        //TODO: [!] uso de assembly
        return false;
    }

    void BuildItemCollection::GetAssemblyTimestamps(std::unordered_map<std::filesystem::path, TimePoint>& timestamps) {
        for(const auto& [key, value] : impl->assemblyTimestamps)
            timestamps[key] = value;
    }

    void BuildItemCollection::DeleteStaleOutputFiles() {
        std::vector<std::filesystem::path> stringList;
        
        for(const auto& previousOutput : impl->previousOutputs) {
            auto buildItem = Find(previousOutput);
            
            if (buildItem != nullptr) {
                stringList.push_back(buildItem.OutputFilename());

                for(size_t index =  0; index < buildItem.ExtraOutputFiles().size(); ++index)
                    stringList.push_back(buildItem.ExtraOutputFiles()[index]);
            }
        }

        for(const auto& previousOutput : impl->previousOutputs)
        {
            if (!Xna::Misc::Contains(stringList, previousOutput))
                PathUtils::SafeDeleteFileAndDirectories(impl->buildSettings.GetAbsolutePath(previousOutput), { impl->buildSettings.IntermediateDirectory, impl->buildSettings.OutputDirectory });
        }
    }

    std::vector<std::pair<std::filesystem::path, std::string>> BuildItemCollection::GetDuplicateWarnings() {
        bool flag = false;
        std::unordered_map<std::filesystem::path, std::vector<BuildItem>> dictionary;

        for(auto& buildItem : *this) {
            if (((int)buildItem.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) == (int)BuildOptions::None) {
                const auto& sourceFilename = buildItem.BuildRequest().SourceFilename;
                if (dictionary.contains(sourceFilename))
                    flag = true;
                else
                    dictionary[sourceFilename] = std::vector<BuildItem>();
                
                dictionary[sourceFilename].push_back(buildItem);
            }
        }

        std::vector<std::pair<std::filesystem::path, std::string>> keyValuePairList;
        
        if (flag) {
            for(const auto&[key, value] : dictionary){
                auto& outputs = value;

                if (outputs.size() > 1) {
                    const auto absolutePath = impl->buildSettings.GetAbsolutePath(key);
                    const auto duplicateWarning = GetDuplicateWarning(outputs);
                    keyValuePairList.push_back(std::pair<std::filesystem::path, std::string>(absolutePath, duplicateWarning));
                }
            }
        }
        return keyValuePairList;
    }

    std::string BuildItemCollection::GetDuplicateWarning(std::vector<BuildItem> const& outputs) {
        std::string stringBuilder;
        stringBuilder.append(std::format(Resources::WarnDuplicateAsset, outputs.size()));

        for(const auto& output : outputs) {
            stringBuilder.append("\n");
            auto str = output.BuildRequest().ProcessorName;
           
            if (str.empty())
                str = Resources::WarnDuplicateAssetNoProcessor;

            stringBuilder.append(std::format(Resources::WarnDuplicateAssetBuiltUsing, output.BuildRequest().ImporterName, str));
            auto duplicateWarningReferees = GetDuplicateWarningReferees(output);

            if (duplicateWarningReferees.size() > 0)
                stringBuilder.append(std::format(Resources::WarnDuplicateAssetReferencedBy, duplicateWarningReferees));
        }
        return std::move(stringBuilder);
    }

    std::string BuildItemCollection::GetDuplicateWarningReferees(BuildItem const& output) {
        std::vector<std::string> stringList;

        for(auto& buildItem : *this) {
            if (((int)buildItem.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) == (int)BuildOptions::None && buildItem.Requests().Contains(output.OutputFilename())) {
                auto str = buildItem.BuildRequest().SourceFilename.string();
                if (!buildItem.BuildRequest().ProcessorName.empty()) {
                    str.append(" (");
                    str.append(buildItem.BuildRequest().ProcessorName);
                    str.append(")");
                }

                stringList.push_back(str);
            }
        }
        std::sort(stringList.begin(), stringList.end()); //TODO: StringComparer.OrdinalIgnoreCase

        const std::string delimiter = ", ";
        std::stringstream ss;
        
        for (size_t i = 0; i < stringList.size(); ++i) {
            if (i != 0) 
                ss << delimiter;
            ss << stringList[i];
        }
        
        return ss.str();
    }
}