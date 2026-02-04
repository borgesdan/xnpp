#ifndef INTERMEDIATE_INTERMEDIATEREADER_HPP
#define INTERMEDIATE_INTERMEDIATEREADER_HPP

#include <memory>

namespace Xna {
	class IntermediateReader {
	public:

		inline IntermediateReader(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(IntermediateReader const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	private:
		struct Implementation {

		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif