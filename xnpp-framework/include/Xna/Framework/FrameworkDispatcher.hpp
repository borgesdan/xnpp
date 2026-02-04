#ifndef XNA_FRAMEWORK_FRAMEWORKDISPATCHER_HPP
#define XNA_FRAMEWORK_FRAMEWORKDISPATCHER_HPP

namespace Xna {
	//FrameworkDispatcher
	struct FrameworkDispatcher final {
		//Updates the status of various framework components (such as power state and media) and raises related events. 
		//If your game does not use the Game class, you must call this method (the recommendation is once per frame) yourself.
		static void Update();
	
		FrameworkDispatcher() = delete;
		FrameworkDispatcher(FrameworkDispatcher&) = delete;
		FrameworkDispatcher(FrameworkDispatcher&&) = delete;
	};
}

#endif