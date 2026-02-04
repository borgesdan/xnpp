#ifndef XNA_COMPILER_CONTENTCOMPILER_HPP
#define XNA_COMPILER_CONTENTCOMPILER_HPP

#include "_ContentCompiler.hpp"
#include <Xna/Internal/TypeRegistry.hpp>

namespace Xna {
	struct ContentCompiler::Implementation {
		std::unordered_map<CSharp::Type, std::shared_ptr<ContentTypeWriter>> writerInstances;
		std::unordered_map<std::shared_ptr<ContentTypeWriter>, std::vector<CSharp::Type>> writerDependencies;
		std::stack<std::shared_ptr<ContentTypeWriter>> initializeContext;
	};
}

#endif