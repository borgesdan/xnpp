#ifndef XNA_CONTENTPIPELINE_CONTENTITEM_HPP
#define XNA_CONTENTPIPELINE_CONTENTITEM_HPP

#include <string>
#include "ContentIdentity.hpp"
#include "OpaqueDataDictionary.hpp"

namespace Xna {
	struct ContentItem {
		virtual ~ContentItem() = default;

		//[ContentSerializer(Optional = true)]
		std::string Name;
		//[ContentSerializer(Optional = true)]
		ContentIdentity Identity;
		//[ContentSerializer(Optional = true)]
		OpaqueDataDictionary OpaqueData;
	};
}

#endif