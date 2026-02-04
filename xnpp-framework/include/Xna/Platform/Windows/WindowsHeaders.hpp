#ifndef XNA_PLATFORM_WINDOWS_WINDOWSHEADERS_HPP
#define XNA_PLATFORM_WINDOWS_WINDOWSHEADERS_HPP

#ifdef PLATFORM_WINDOWS

#define NOMINMAX

//DirectX
#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#define NO_D3D11_DEBUG_NAME
#endif
#include "dxgi.h"
#include "d3d11.h"
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11_3.h>
#include <d3d11_4.h>
#include <d3d11shader.h>
//#include <d3d11shadertracing.h>
#include <d3dcommon.h>
#include <d3dcsx.h>
//DirectXTK
#include <DirectXMath.h>
#include <Audio.h>
#include <BufferHelpers.h>
#include <CommonStates.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>
#include <Effects.h>
#include <GamePad.h>
#include <GeometricPrimitive.h>
#include <GraphicsMemory.h>
#include <Keyboard.h>
#include <Model.h>
#include <Mouse.h>
#include <PostProcess.h>
#include <PrimitiveBatch.h>
#include <ScreenGrab.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
//Windows
#include <Windows.h>
#include <windowsx.h>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <wrl\implements.h>
#include <wincodec.h>
#include <shlwapi.h>
#include <comdef.h> //comsuppw.lib

//Audio
#include <xaudio2.h>
#include <x3daudio.h>
#include <mfmediaengine.h> //MediaPlayer
#include <mfapi.h> //MediaPlayer
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>

#endif
#endif