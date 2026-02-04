#ifndef XNA_INTERMEDIATE_XMLLISTREADER_HPP
#define XNA_INTERMEDIATE_XMLLISTREADER_HPP

#include "IntermediateReader.hpp"
#include <string>
#include <vector>

namespace Xna {
	class XmlListReader {
	public:
		XmlListReader(IntermediateReader const& reader) {
			//TODO: [!] não implementado
		}

		std::string ReadString() {
			//TODO: [!] não implementado
			return {};
		}

		inline bool AtEnd() const {
			return impl->atEnd;
		}

		inline XmlListReader(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(XmlListReader const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	private:
		struct Implementation {
			IntermediateReader reader{nullptr};
			std::vector<string> enumerator;
			bool atEnd{ false };
		};

		std::shared_ptr<Implementation> impl;		

		static constexpr char listSeparator[4] = { ' ', '\t', '\r', '\n' };
	};
}

#endif