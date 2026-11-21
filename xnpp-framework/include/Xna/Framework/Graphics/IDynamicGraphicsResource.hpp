#pragma once

#include <Xna/CSharp/Event.hpp>

namespace Xna {
	//internal class
	struct IDynamicGraphicsResource {
	public:
		virtual ~IDynamicGraphicsResource() = default;
		virtual IEvent<EventArgs>& ContentLost() const = 0;
		virtual bool IsContentLost() const = 0;
		virtual void SetContentLost(bool IsContentLost) = 0;
	};
}