//CSharp
#include "CSharp/Event.hpp"
#include "CSharp/IO.hpp"
#include "CSharp/TimeSpan.hpp"

//Framework
#include "Framework/Volume.hpp"
#include "Framework/Color.hpp"
#include "Framework/Curve.hpp"
#include "Framework/FrameworkDispatcher.hpp"
#include "Framework/MathHelper.hpp"
#include "Framework/MathTypes.hpp"

//Game
#include "Framework/Game/Game.hpp"
#include "Framework/Game/GameServiceContainer.hpp"
#include "Framework/Game/GameWindow.hpp"
#include "Framework/Game/GraphicsDeviceInformation.hpp"
#include "Framework/Game/GraphicsDeviceManager.hpp"
#include "Framework/Game/IGraphicsDeviceManager.hpp"

//Graphics
#include "Framework/Graphics/BlendState.hpp"
#include "Framework/Graphics/DepthStencilState.hpp"
#include "Framework/Graphics/DisplayMode.hpp"
#include "Framework/Graphics/GraphicsAdapter.hpp"
#include "Framework/Graphics/GraphicsDevice.hpp"
#include "Framework/Graphics/GraphicsResource.hpp"
#include "Framework/Graphics/IGraphicsDeviceService.hpp"
#include "Framework/Graphics/IVertexType.hpp"
#include "Framework/Graphics/Model.hpp"
#include "Framework/Graphics/OcclusionQuery.hpp"
#include "Framework/Graphics/PresentationParameters.hpp"
#include "Framework/Graphics/RasterizerState.hpp"
#include "Framework/Graphics/RenderTargetBinding.hpp"
#include "Framework/Graphics/RenderTargetCube.hpp"
#include "Framework/Graphics/RenderTarget2D.hpp"
#include "Framework/Graphics/SamplerState.hpp"
#include "Framework/Graphics/Shared.hpp"
#include "Framework/Graphics/SpriteBatch.hpp"
#include "Framework/Graphics/SpriteFont.hpp"
#include "Framework/Graphics/Texture.hpp"
#include "Framework/Graphics/TextureCollection.hpp"
#include "Framework/Graphics/TextureCube.hpp"
#include "Framework/Graphics/Texture2D.hpp"
#include "Framework/Graphics/Texture3D.hpp"
#include "Framework/Graphics/VertexDeclaration.hpp"
#include "Framework/Graphics/VertexElement.hpp"
#include "Framework/Graphics/Viewport.hpp"

//Input
#include "Framework/Input/GamePad.hpp"
#include "Framework/Input/Keyboard.hpp"
#include "Framework/Input/Mouse.hpp"
#include "Framework/Input/Shared.hpp"

//Media
#include "Framework/Media/Album.hpp"
#include "Framework/Media/Artist.hpp"
#include "Framework/Media/Genre.hpp"
#include "Framework/Media/MediaPlayer.hpp"
#include "Framework/Media/Song.hpp"
#include "Framework/Media/VisualizationData.hpp"

//Audio
#include "Framework/Audio/DynamicSoundEffectInstance.hpp"
#include "Framework/Audio/Shared.hpp"
#include "Framework/Audio/SoundEffect.hpp"
#include "Framework/Audio/SoundEffectInstance.hpp"

//Content
#include "Framework/Content/ContentManager.hpp"
