#ifndef XNA_FRAMEWORK_FRAMEWORKDISPATCHER_HPP
#define XNA_FRAMEWORK_FRAMEWORKDISPATCHER_HPP

#include "Xna/Internal/Export.hpp"

namespace Xna {
	//FrameworkDispatcher
	struct FrameworkDispatcher final {
		//Updates the status of various framework components (such as power state and media) and raises related events. 
		//If your game does not use the Game class, you must call this method (the recommendation is once per frame) yourself.
		XNPP_API static void Update();
	
		FrameworkDispatcher() = delete;
		FrameworkDispatcher(FrameworkDispatcher&) = delete;
		FrameworkDispatcher(FrameworkDispatcher&&) = delete;
	};
}

#endif