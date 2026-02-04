#ifndef XNA_CONTENTPIPELINE_CONTENTIMPORTER_HPP
#define XNA_CONTENTPIPELINE_CONTENTIMPORTER_HPP

#include <filesystem>
#include <any>
#include "ContentImporterContext.hpp"
#include "ContentImporterAttribute.hpp"
#include <Xna/Internal/TypeRegistry.hpp>

namespace Xna {

	struct IContentImporter {
		virtual ~IContentImporter() = default;
		virtual std::optional<ContentImporterAttribute> GetAttribute() const = 0;	
		virtual std::any Import(std::filesystem::path const& filename, ContentImporterContext& context) = 0;
	};	

	template<typename T>
	struct ContentImporter : public IContentImporter {
		virtual T ImportT(std::filesystem::path const& filename, ContentImporterContext& context) = 0;
		
		std::optional<ContentImporterAttribute> GetAttribute() const override {
			return std::nullopt;
		}
	
		std::any Import(std::filesystem::path const& filename, ContentImporterContext& context) override {
			const auto value = ImportT(filename, context);
			const std::any obj = value;
			return obj;
		}
	};
}

#endif