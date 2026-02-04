#ifndef XNA_FRAMEWORK_GRAPHICS_IGRAPHICSDEVICESERVICE_HPP
#define XNA_FRAMEWORK_GRAPHICS_IGRAPHICSDEVICESERVICE_HPP

#include "Xna/CSharp/Event.hpp"
#include <optional>

namespace Xna {
    class GraphicsDevice;

    //Defines a mechanism for retrieving GraphicsDevice objects.
	struct IGraphicsDeviceService {
    public:
        virtual ~IGraphicsDeviceService() = default;

        //Retrieves a graphcs device.
        virtual std::optional<Xna::GraphicsDevice> GraphicsDevice() const = 0;

        //The event that occurs when a graphics device is created.
        virtual CSharp::Event<CSharp::EventArgs> DeviceCreated() const = 0;
        //The event that occurs when a graphics device is in the process of resetting.
        virtual CSharp::Event<CSharp::EventArgs> DeviceResetting() const = 0;
        //The event that occurs when a graphics device is reset.
        virtual CSharp::Event<CSharp::EventArgs> DeviceReset() const = 0;
        //The event that occurs when a graphics device is disposing.
        virtual CSharp::Event<CSharp::EventArgs> DeviceDisposing() const = 0;
	};
}

#endif