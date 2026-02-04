#ifndef XNA_FRAMEWORK_GRAPHICS__GRAPHICSDEVICE_HPP
#define XNA_FRAMEWORK_GRAPHICS__GRAPHICSDEVICE_HPP


#include <memory>
#include <cstdint>
#include <vector>
#include "Shared.hpp"
#include "../Rectangle.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/Event.hpp"
#include "PresentationParameters.hpp"

namespace Xna {
	struct Color;
	struct Viewport;
	struct PresentationParameters;
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class SamplerStateCollection;
	class RenderTarget2D;
	class GraphicsAdapter;
	class TextureCollection;
	class RenderTargetBinding;
	class VertexBufferBinding;
	class RenderTargetCube;
	class VertexBuffer;
	class VertexDeclaration;

	//Describes the status of the GraphicsDevice.
	enum class GraphicsDeviceStatus
	{
		//The device is normal.
		Normal,
		//The device has been lost.
		Lost,
		//The device has not been reset.
		NotReset,
	};

	//Performs primitive-based rendering, creates resources, handles system-level variables, and creates shaders.
	class GraphicsDevice {
	public:
		//
		//Constructors
		//

		//Creates an instance of this object.
		GraphicsDevice(GraphicsAdapter const& adapter, GraphicsProfile const& graphicsProfile, PresentationParameters const& presentationParameters);

		//
		//Properties
		//

		//Gets the graphics adapter.
		inline GraphicsAdapter Adapter() const;
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		inline Xna::BlendState BlendState() const;
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		void BlendState(Xna::BlendState const& value);
		//Gets or sets the color used for a constant-blend factor during alpha blending. The default value is White.
		inline Color BlendFactor() const;
		//Gets or sets the color used for a constant-blend factor during alpha blending. The default value is White.
		void BlendFactor(Color const& color);
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		inline Xna::DepthStencilState DepthStencilState() const;
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		void DepthStencilState(Xna::DepthStencilState const& value);
		//Gets or sets a bitmask controlling modification of the samples in a multisample render target. The default value is -1 (0xffffffff).
		inline int32_t MultiSampleMask() const;
		//Gets or sets a bitmask controlling modification of the samples in a multisample render target. The default value is -1 (0xffffffff).
		void MultiSampleMask(int32_t value);
		//Gets the presentation parameters associated with this graphics device.
		inline Xna::PresentationParameters& PresentationParameters();
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		inline Xna::RasterizerState RasterizerState() const;
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		void RasterizerState(Xna::RasterizerState const& value);
		//Gets or sets a reference value for stencil testing. The default value is zero.
		inline int32_t ReferenceStencil() const;
		//Gets or sets a reference value for stencil testing. The default value is zero.
		void ReferenceStencil(int32_t value);
		//Retrieves a collection of SamplerState objects for the current GraphicsDevice. 
		inline SamplerStateCollection SamplerStates() const;
		//Gets or sets the rectangle used for scissor testing. By default, the size matches the render target size.
		inline Rectangle ScissorRectangle() const;
		//Gets or sets the rectangle used for scissor testing. By default, the size matches the render target size.
		void ScissorRectangle(Rectangle const& value);
		//Gets the collection of textures that have been assigned to the texture stages of the device.
		inline TextureCollection Textures() const;
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		inline Xna::Viewport Viewport() const;
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		void Viewport(Xna::Viewport const& viewport);

		inline Xna::GraphicsProfile GraphicsProfile() const { return Xna::GraphicsProfile::HiDef; }
		//
		//Methods
		// 

		//Clears resource buffers. 
		inline void Clear(Color const& color);
		//Clears resource buffers. 
		void Clear(ClearOptions options, Color const& color, float depth, int32_t stencil);
		//Renders the specified geometric primitive, based on indexing into an array of vertices.
		void DrawIndexedPrimitives(PrimitiveType primitiveType, int32_t baseVertex, int32_t minVertexIndex, int32_t numVertices, int32_t startIndex, int32_t primitiveCount) {} // TODO
		//Draws a series of instanced models.
		void DrawInstancedPrimitives(PrimitiveType primitiveType, int32_t baseVertex, int32_t minVertexIndex, int32_t numVertices,
			int32_t startIndex, int32_t primitiveCount, int32_t instanceCount) { } // TODO
		//Renders a sequence of non-indexed geometric primitives of the specified type from the current set of data input streams.
		void DrawPrimitives(PrimitiveType primitiveType, int32_t startVertex, int32_t primitiveCount) {} // TODO
		//Draws primitives.
		template <typename T>
		void DrawUserPrimitives(PrimitiveType primitiveType, std::vector<T> const& vertexData, int32_t vertexOffset, int32_t primitiveCount) { } //TODO
		//Draws primitives.
		template <typename T>
		void DrawUserPrimitives(PrimitiveType primitiveType, std::vector<T> const& vertexData, int32_t vertexOffset, int32_t primitiveCount, VertexDeclaration vertexDeclaration) {	} //TODO
		//Gets the contents of the back buffer.
		template <typename T>
		void GetBackBufferData(std::optional<Rectangle> const& rect, std::vector<T> const& data, int32_t startIndex, int32_t elementCount) { } //TODO
		//Gets the contents of the back buffer.
		template <typename T>
		void GetBackBufferData(std::vector<T> const& data) { } //TODO
		//Gets a render target surface.
		inline std::vector<RenderTargetBinding> GetRenderTargets();
		//Gets the vertex buffers.
		inline std::vector<VertexBufferBinding> GetVertexBuffers();
		
		//Presents the display with the contents of the next buffer in the sequence of back buffers owned by the GraphicsDevice.
		inline void Present() { Present(std::nullopt, std::nullopt, 0); }
		//Specifies the window target for a presentation and presents the display with the contents of the next buffer in the sequence of back buffers owned by the GraphicsDevice.
		void Present(intptr_t overrideWindowHandle) { Present(std::nullopt, std::nullopt, overrideWindowHandle); }
		//Specifies the window target for a presentation and presents the display with the contents of the next buffer in the sequence of back buffers owned by the GraphicsDevice.
		void Present(std::optional<Rectangle> const& sourceRec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle);
		
		//Resets the presentation parameters for the current GraphicsDevice.
		inline void Reset();
		//Resets the presentation parameters for the current GraphicsDevice.
		inline void Reset(Xna::PresentationParameters const& presentationParameters);
		//Resets the presentation parameters for the current GraphicsDevice.
		inline void Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter);

		//Sets a new render target for this GraphicsDevice. 
		void SetRenderTarget(RenderTarget2D const& renderTarget) {
			//TODO
		}
		//Sets a new render target for this GraphicsDevice.
		void SetRenderTarget(RenderTargetCube const& renderTarget, CubeMapFace const& cubeMapFace) {
			//TODO
		}
		//Sets an array of render targets.
		void SetRenderTargets(std::vector<RenderTargetBinding> const& renderTargets) {
			//TODO
		}
		//Sets or binds a vertex buffer to the device.
		void SetVertexBuffer(VertexBuffer const& vertexBuffer, int32_t vertexOffset = 0) {
			//TODO
		}
		//Sets or binds a vertex buffer to the device.
		void SetVertexBuffers(std::vector<VertexBufferBinding> const& vertexBuffers) {
			//TODO
		}

		Xna::GraphicsDeviceStatus GraphicsDeviceStatus() const {
			//TODO
			return Xna::GraphicsDeviceStatus::Normal;
		}

		inline Xna::CSharp::Event<CSharp::EventArgs> DeviceResetting();
		inline CSharp::Event<CSharp::EventArgs> DeviceReset();
		inline CSharp::Event<CSharp::EventArgs> DeviceLost();
		inline CSharp::Event<CSharp::EventArgs> Disposing();

		inline GraphicsDevice(std::nullptr_t) { implGraphicsDevice = nullptr; }
		inline bool operator==(GraphicsDevice const& other) const noexcept { return implGraphicsDevice == other.implGraphicsDevice; }
		inline bool operator==(std::nullptr_t) const noexcept { return implGraphicsDevice == nullptr; }
		inline explicit operator bool() const noexcept { return implGraphicsDevice != nullptr; }

	private:
		void CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters);
		inline RenderTarget2D CreateBackBufferRenderTarget() const;
	
		struct Implementation;
		std::shared_ptr<Implementation> implGraphicsDevice;	

		friend struct Platform;
	};
}
#endif