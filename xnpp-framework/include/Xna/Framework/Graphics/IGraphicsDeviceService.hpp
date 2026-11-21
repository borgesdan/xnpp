#pragma once

#include <Xna/CSharp/Event.hpp>

namespace Xna {
    class GraphicsDevice;

    //Defines a mechanism for retrieving GraphicsDevice objects.
	struct IGraphicsDeviceService {
    public:
        virtual ~IGraphicsDeviceService() = default;

        //Retrieves a graphcs device.
        virtual Xna::GraphicsDevice* GraphicsDevice() const = 0;

        //The event that occurs when a graphics device is created.
        virtual IEvent<EventArgs>& DeviceCreated() const = 0;
        //The event that occurs when a graphics device is in the process of resetting.
        virtual IEvent<EventArgs>& DeviceResetting() const = 0;
        //The event that occurs when a graphics device is reset.
        virtual IEvent<EventArgs>& DeviceReset() const = 0;
        //The event that occurs when a graphics device is disposing.
        virtual IEvent<EventArgs>& DeviceDisposing() const = 0;
	};
}