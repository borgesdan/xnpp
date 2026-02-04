#include "Xna/Framework/Graphics/GraphicsContentHelper.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/IGraphicsDeviceService.hpp"
#include "Xna/Framework/Content/ContentReader.hpp"

#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	GraphicsDevice GraphicsContentHelper::GraphicsDeviceFromContentReader(ContentReader& contentReader) {
        auto iGraphicsDevice =  contentReader.ContentManager().ServiceProvider().GetService<IGraphicsDeviceService>();

        if (!iGraphicsDevice)
            throw CSharp::InvalidOperationException("NoGraphicsDeviceContent");

        auto graphicsDevice = iGraphicsDevice->GraphicsDevice();

        if(!graphicsDevice)
            throw CSharp::InvalidOperationException("NoGraphicsDeviceContent");

        const auto deviceProfile = graphicsDevice->GraphicsProfile();
        const auto graphicsProfile = static_cast<GraphicsProfile>(contentReader.impl->graphicsProfile);
        
        if (!GraphicsContentHelper::IsProfileCompatible(deviceProfile, graphicsProfile))
            throw CSharp::InvalidOperationException("BadXnbGraphicsProfile");

        return *graphicsDevice;
	}

    bool GraphicsContentHelper::IsProfileCompatible(GraphicsProfile deviceProfile, GraphicsProfile contentProfile) {
        switch (deviceProfile)
        {
        case GraphicsProfile::Reach:
            return contentProfile == GraphicsProfile::Reach;
        case GraphicsProfile::HiDef:
            return contentProfile == GraphicsProfile::Reach || contentProfile == GraphicsProfile::HiDef;
        default:
            return false;
        }
    }
}