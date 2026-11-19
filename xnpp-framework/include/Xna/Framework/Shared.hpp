#ifndef XNA_FRAMEWORK_SHARED_HPP
#define XNA_FRAMEWORK_SHARED_HPP

namespace Xna {
	

	

	//Defines the display orientation.
    enum class DisplayOrientation
    {
		//The default orientation.
        Default = 0,
		//The display is rotated counterclockwise into a landscape orientation. Width is greater than height.
        LandscapeLeft = 1,
		//The display is rotated clockwise into a landscape orientation. Width is greater than height.
        LandscapeRight = 2,
		//The display is rotated as portrait, where height is greater than width.
        Portrait = 4,
    };

	
}

#endif