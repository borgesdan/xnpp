#include "Xna/ContentPipeline/OpaqueDataDictionary.hpp"
#include <stdexcept>

namespace Xna {
	std::string OpaqueDataDictionary::GetContentAsXml() const {
		//TODO: uso de XML
		throw std::runtime_error("OpaqueDataDictionary::GetContentAsXml is not implemented.");

		return {};
	}

	void OpaqueDataDictionary::AddItem(std::string const& key, std::any const& value) {
		impl->contentAsXml = std::nullopt;
		AddItem(key, value);
	}

	void OpaqueDataDictionary::ClearItems() {
		impl->contentAsXml = std::nullopt;
		ClearItems();
	}

	bool OpaqueDataDictionary::RemoveItem(std::string const& key) {
		impl->contentAsXml = std::nullopt;
		return RemoveItem(key);
	}

	void OpaqueDataDictionary::SetItem(std::string const& key, std::any const& value) {
		impl->contentAsXml = std::nullopt;
		SetItem(key, value);
	}
}