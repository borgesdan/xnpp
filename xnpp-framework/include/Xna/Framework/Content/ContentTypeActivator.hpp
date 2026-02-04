#ifndef XNA_FRAMEWORK_CONTENT_CONTENTTYPEACTIVATOR_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTTYPEACTIVATOR_HPP

#include <memory>
#include <vector>
#include <functional>
#include <any>
#include <map>
#include "Xna/CSharp/Type.hpp"
#include "ContentReader.hpp"

namespace Xna {	

	struct ContentTypeReaderParser {
		static inline void Add(std::string const& csharpTypeName, CSharp::Type const& currentType) {
			CSharp::Type::NamedTypes()[csharpTypeName] = currentType;
		}

		static inline CSharp::Type* Parse(std::string const& csharpTypeName) {
			auto sub = csharpTypeName.substr(0, csharpTypeName.find(","));

			if(CSharp::Type::NamedTypes().contains(sub)) {
				return &CSharp::Type::NamedTypes()[sub];
			}

			return nullptr;
		}
	};
}

#endif