#ifndef XNA_FRAMEWORK_GAME_IGRAPHICSDEVICEMANAGER_HPP
#define XNA_FRAMEWORK_GAME_IGRAPHICSDEVICEMANAGER_HPP

namespace Xna {
	//Defines the interface for an object that manages a GraphicsDevice.
	class IGraphicsDeviceManager {
	public:
		virtual ~IGraphicsDeviceManager() = default;
		//Called to ensure that the device manager has created a valid device.
		virtual void CreateDevice() = 0;
		//Starts the drawing of a frame.
		virtual bool BeginDraw() = 0;
		//Called by the game at the end of drawing; presents the final rendering.
		virtual void EndDraw() = 0;
	};
}

#endif