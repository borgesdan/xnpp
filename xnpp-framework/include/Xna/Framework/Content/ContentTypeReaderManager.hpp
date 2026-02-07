#ifndef XNA_FRAMEWORK_CONTENT_CONTENTTYPEREADERMANAGER_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTTYPEREADERMANAGER_HPP

#include "_ContentTypeReaderManager.hpp"
#include "ContentReader.hpp"
#include "ContentTypeReader.hpp"
#include <unordered_map>
#include <optional>

namespace Xna {
	template <typename T>
	void ContentTypeReaderManager::RollbackAddReader(std::map<T, std::shared_ptr<ContentTypeReader>>& dictionary, ContentTypeReader const& reader) {	
		
		for (auto it = dictionary.begin(); it != dictionary.end();) {
			if (it->second.get() == &reader)
				dictionary.erase(it);
			else
				++it;
		}
	}
}

#endif