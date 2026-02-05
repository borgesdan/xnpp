#ifndef XNA_FRAMEWORK_TITLECONTAINER_HPP
#define XNA_FRAMEWORK_TITLECONTAINER_HPP

#include "Xna/CSharp/IO/Stream.hpp"
#include "Xna/Internal/Export.hpp"
#include <memory>
#include <filesystem>

namespace Xna {
	//Provides file stream access to the title's default storage location.
	struct XNPP_API TitleContainer final {
	public:
		//Returns a stream to an existing file in the default title storage location.
		static std::unique_ptr<CSharp::Stream> OpenStream(std::filesystem::path const& name);

		TitleContainer() = delete;
		TitleContainer(TitleContainer&) = delete;
		TitleContainer(TitleContainer&&) = delete;

	private:
		static inline std::filesystem::path GetCleanPath(std::filesystem::path const& path) { return path.lexically_normal(); }

		static inline bool IsPathAbsolute(std::filesystem::path const& path) {
			const auto _path = GetCleanPath(path);
			return IsCleanPathAbsolute(_path);
		}

		static int32_t CollapseParentDirectory(std::string& path, int32_t position, size_t removeLength);
		static inline bool IsCleanPathAbsolute(std::filesystem::path const& path) { return path.is_absolute(); }

		friend class ContentManager;
		friend class ContentReader;
	};
}

#endif