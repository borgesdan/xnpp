#ifndef XNA_FRAMEWORK_GRAPHICS_IDYNAMICGRAPHICSRESOURCE_HPP
#define XNA_FRAMEWORK_GRAPHICS_IDYNAMICGRAPHICSRESOURCE_HPP

#include "Xna/CSharp/Event.hpp"

namespace Xna {
	//internal class
	struct IDynamicGraphicsResource {
	public:
		virtual ~IDynamicGraphicsResource() = default;
		virtual CSharp::Event<CSharp::EventArgs> ContentLost() const = 0;
		virtual bool IsContentLost() const = 0;
		virtual void SetContentLost(bool IsContentLost) = 0;
	};
}

#endif