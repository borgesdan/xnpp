#ifndef XNA_CONTENTPIPELINE_TIMESTAMPCACHE_HPP
#define XNA_CONTENTPIPELINE_TIMESTAMPCACHE_HPP

#include <filesystem>
#include <memory>
#include <unordered_map>
#include "Shared.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	class TimestampCache {
	public:
		inline TimestampCache() { impl = std::make_shared<Implementation>(); }

		XNPP_API TimePoint GetTimestamp(std::filesystem::path const& filename);
		XNPP_API void Remove(std::filesystem::path const& filename);
		XNPP_API bool FileExists(std::filesystem::path const& filename);

		inline TimestampCache(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(TimestampCache const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			std::unordered_map<std::filesystem::path, TimePoint> timestampCache;
			std::unordered_map<std::filesystem::path, bool> existsCache;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif