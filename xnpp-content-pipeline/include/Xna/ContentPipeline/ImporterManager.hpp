#ifndef XNA_CONTENTPIPELINE_IMPORTMANAGER_HPP
#define XNA_CONTENTPIPELINE_IMPORTMANAGER_HPP

#include <memory>
#include <filesystem>
#include <any>
#include <string>
#include <unordered_map>
#include "ContentImporterAttribute.hpp"
#include "ContentImporterContext.hpp"
#include "ContentImporter.hpp"
#include <Xna/CSharp/Type.hpp>

namespace Xna {
	

	class ImporterManager {
	public:
		ImporterManager();

		std::string GuessFromFilename(std::filesystem::path const& filename);
		ContentImporterAttribute GetImporterAttribute(std::string& importerName);
		std::shared_ptr<IContentImporter> GetInstance(std::string const& importerName);

		inline ImporterManager(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ImporterManager const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			//TODO: [!] remover tudo isso e implementar um cache
			std::unordered_map<std::string, CSharp::Type> importerTypes;
			std::unordered_map<std::string, ContentImporterAttribute> importerAttributes;
			std::unordered_map<std::string, CSharp::Type> importerOutputTypes;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif