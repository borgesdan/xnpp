#ifndef XNA_CONTENTPIPELINE_XNACONTENTIMPORTERCONTEXT_HPP
#define XNA_CONTENTPIPELINE_XNACONTENTIMPORTERCONTEXT_HPP

#include "ContentImporterContext.hpp"
#include "BuildItem.hpp"
#include "BuildCoordinator.hpp"
#include "ContentBuildLogger.hpp"
#include <string>
#include <memory>
#include <filesystem>

namespace Xna {
	class XnaContentImporterContext : public ContentImporterContext {
	public:
		~XnaContentImporterContext() override = default;

		inline XnaContentImporterContext(
			BuildCoordinator const& buildCoordinator,
			BuildItem const& buildItem,
			ContentBuildLogger const& logger) {
			impl = std::make_shared<Implementation>();
			impl->buildCoordinator = buildCoordinator;
			impl->buildItem = buildItem;
			impl->logger = logger;
		}

		ContentBuildLogger Logger() const override { return impl->logger; }
		void AddDependency(std::filesystem::path const& filename) const override { return impl->buildCoordinator.AddDependency(impl->buildItem, filename); }
		std::filesystem::path OutputDirectory() const override { return impl->buildCoordinator.Settings().OutputDirectory; }
		std::filesystem::path IntermediateDirectory() const override { return impl->buildCoordinator.Settings().IntermediateDirectory; }

		inline XnaContentImporterContext(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(XnaContentImporterContext const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			BuildCoordinator buildCoordinator{ nullptr };
			BuildItem buildItem{ nullptr };
			ContentBuildLogger logger{ nullptr };
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif