#ifndef XNA_FRAMEWORK_GRAPHICS_SHARED_HPP
#define XNA_FRAMEWORK_GRAPHICS_SHARED_HPP

namespace Xna {
	//Defines color blending factors.
	enum class Blend {
		//Each component of the color is multiplied by (0, 0, 0, 0). 
		Zero,
		//Each component of the color is multiplied by (1, 1, 1, 1)
		One,
		//Each component of the color is multiplied by the source color. This can be represented as (Rs, Gs, Bs, As), 
		// where R, G, B, and A respectively stand for the red, green, blue, and alpha source values. 
		SourceColor,
		//Each component of the color is multiplied by the inverse of the source color. 
		//This can be represented as (1 − Rs, 1 − Gs, 1 − Bs, 1 − As) where R, G, B, and A respectively stand for the red, green, blue, and alpha destination values. 
		InverseSourceColor,
		//Each component of the color is multiplied by the alpha value of the source. 
		//This can be represented as (As, As, As, As), where As is the alpha source value. 
		SourceAlpha,
		//Each component of the color is multiplied by the inverse of the alpha value of the source. 
		//This can be represented as (1 − As, 1 − As, 1 − As, 1 − As), where As is the alpha destination value. 
		InverseSourceAlpha,
		//Each component of the color is multiplied by the alpha value of the destination.
		//This can be represented as (Ad, Ad, Ad, Ad), where Ad is the destination alpha value. 
		DestinationAlpha,
		//Each component of the color is multiplied by the inverse of the destination color.
		//This can be represented as (1 − Rd, 1 − Gd, 1 − Bd, 1 − Ad), where Rd, Gd, Bd, and Ad respectively stand for the red, green, blue, and alpha destination values.
		InverseDestinationAlpha,
		//Each component color is multiplied by the destination color.
		//This can be represented as (Rd, Gd, Bd, Ad), where R, G, B, and A respectively stand for red, green, blue, and alpha destination values. 
		DestinationColor,
		//Each component of the color is multiplied by the inverse of the destination color.
		// This can be represented as (1 − Rd, 1 − Gd, 1 − Bd, 1 − Ad), where Rd, Gd, Bd, and Ad respectively stand for the red, green, blue, and alpha destination values. 
		InverseDestinationColor,
		//Each component of the color is multiplied by either the alpha of the source color, or the inverse of the alpha of the source color, whichever is greater.
		// This can be represented as (f, f, f, 1), where f = min(A, 1 − Ad). 
		SourceAlphaSaturation,
		//Each component of the color is multiplied by a constant set in BlendFactor. 
		BlendFactor,
		//Each component of the color is multiplied by the inverse of a constant set in BlendFactor. 
		InverseBlendFactor,
		Source1Color,
		InverseSource1Color,
		Source1Alpha,
		InverseSource1Alpha
	};

	//Defines how to combine a source color with the destination color already on the render target for color blending.
	enum class BlendOperation {
		//The result is the destination added to the source.
		//Result = (Source Color * Source Blend) + (Destination Color * Destination Blend)
		Add = 0,
		//The result is the destination subtracted from the source.
		//Result = (Source Color * Source Blend) −(Destination Color * Destination Blend)
		Subtract = 1,
		//The result is the source subtracted from the destination.
		//Result = (Destination Color * Destination Blend) −(Source Color * Source Blend)
		ReverseSubtract = 2,
		//The result is the minimum of the source and destination.
		//Result = min((Source Color * Source Blend), (Destination Color * Destination Blend))
		Min = 3,
		//The result is the maximum of the source and destination.
		//Result = max((Source Color * Source Blend), (Destination Color * Destination Blend))
		Max = 4,
	};

	//Specifies special usage of the buffer contents.
	enum class BufferUsage
	{
		//Indicates that the vertex buffer is not used.
		None = 0,
		//Indicates that the application only writes to the vertex buffer. 
		//If specified, the driver chooses the best memory location for efficient writing and rendering. 
		//Attempts to read from a write-only vertex buffer fail.
		WriteOnly = 1,
	};

	//Defines the color channels that can be chosen for a per-channel write to a render target color buffer. 
	enum class ColorWriteChannels {
		//Red channel of a buffer.
		Red,
		//Green channel of a buffer.
		Green,
		//Blue channel of a buffer.
		Blue,
		//Alpha channel of a buffer.
		Alpha,
		//All buffer channels.
		All,
		//No channel selected.
		None
	};

	//Defines comparison functions that can be chosen for alpha, stencil, or depth-buffer tests. 
	enum class ComparisonFunction {
		//Always fail the test.
		Never,
		//Accept the new pixel if its value is less than the value of the current pixel.
		Less,
		//Accept the new pixel if its value is equal to the value of the current pixel.
		Equal,
		//Accept the new pixel if its value is less than or equal to the value of the current pixel.
		LessEquals,
		//Accept the new pixel if its value is greater than the value of the current pixel. 
		Greater,
		//Accept the new pixel if its value does not equal the value of the current pixel.
		NotEqual,
		//Accept the new pixel if its value is greater than or equal to the value of the current pixel.
		GreaterEqual,
		//Always pass the test.
		Always
	};	

	//Defines the faces of a cube map in the TextureCube class type.
	enum class CubeMapFace {
		//Positive x-face of the cube map.
		PositiveX,
		//Negative x-face of the cube map.
		NegativeX,
		//Positive y-face of the cube map.
		PositiveY,
		//Negative y-face of the cube map.
		NegativeY,
		//Positive z-face of the cube map.
		PositiveZ,
		//Negative z-face of the cube map.
		NegativeZ
	};

	//Defines winding orders that may be used to identify back faces for culling. 
	enum class CullMode {
		//Do not cull back faces. 
		None,
		//Cull back faces with clockwise vertices. 
		CullClockwiseFace,
		//Cull back faces with counterclockwise vertices. 
		CullCounterClockwiseFace,
	};

	//Defines the format of data in a depth-stencil buffer. 
	enum class DepthFormat {
		//Do not create a depth buffer.
		None,
		//A buffer that contains 16-bits of depth data.
		Depth16,
		//A buffer that contains 24-bits of depth data.
		Depth24,
		//A 32 bit buffer that contains 24 bits of depth data and 8 bits of stencil data.
		Depth24Stencil8,

		//A 32 bit buffer, greater accuracy (floating point) and better depth distribution (reduces z-fighting at longer distances)
		Depth32
	};

	//Describes options for filling the vertices and lines that define a primitive. 
	enum class FillMode
	{
		//Draw lines connecting the vertices that define a primitive face.
		WireFrame,
		//Draw solid faces for each primitive.
		Solid,
	};

	enum class IndexElementSize
	{
		SixteenBits,
		ThirtyTwoBits,
	};

	//Defines various types of surface formats. 
	enum class SurfaceFormat {
		//(Unsigned format) 32-bit ARGB pixel format with alpha, using 8 bits per channel.
		Color = 0,
		//(Unsigned format) 16-bit BGR pixel format with 5 bits for blue, 6 bits for green, and 5 bits for red.
		Bgr565 = 1,
		//(Unsigned format) 16-bit BGRA pixel format where 5 bits are reserved for each color and 1 bit is reserved for alpha.
		Bgra5551 = 2,
		//(Unsigned format) 16-bit BGRA pixel format with 4 bits for each channel.
		Bgra4444 = 3,
		//DXT1 compression texture format. 
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt1 = 4,
		//DXT3 compression texture format.
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt3 = 5,
		//DXT5 compression texture format.
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt5 = 6,
		//Signed format) 16-bit bump-map format using 8 bits each for u and v data.
		NormalizedByte2 = 7,
		//Signed format) 32-bit bump-map format using 8 bits for each channel.
		NormalizedByte4 = 8,
		//(Unsigned format) 32-bit RGBA pixel format using 10 bits for each color and 2 bits for alpha.
		Rgba1010102 = 9,
		//Unsigned format) 32-bit pixel format using 16 bits each for red and green. 
		Rg32 = 10,
		//(Unsigned format) 64-bit RGBA pixel format using 16 bits for each component.
		Rgba64 = 11,
		//(Unsigned format) 8-bit alpha only.
		Alpha8 = 12,
		//(IEEE format) 32-bit float format using 32 bits for the red channel.
		Single = 13,
		//(IEEE format) 64-bit float format using 32 bits for the red channel and 32 bits for the green channel.
		Vector2 = 14,
		//(IEEE format) 128-bit float format using 32 bits for each channel (alpha, blue, green, red).
		Vector4 = 15,
		//(Floating-point format) 16-bit float format using 16 bits for the red channel.
		HalfSingle = 16,
		//(Floating-point format) 32-bit float format using 16 bits for the red channel and 16 bits for the green channel.
		HalfVector2 = 17,
		//(Floating-point format) 64-bit float format using 16 bits for each channel (alpha, blue, green, red).
		HalfVector4 = 18,
		//(Floating-point format) for high dynamic range data.
		HdrBlendable = 19,
		//Not implemented.
		Unknown,
	};	

	//Identifies the set of supported devices for the game based on device capabilities.
	enum class GraphicsProfile {
		//Use a limited set of graphic features and capabilities, allowing the game to support the widest variety of devices, including all Windows-based computers and Windows Phone.
		Reach,
		//Use the largest available set of graphic features and capabilities to target devices, such as an Xbox 360 console and a Windows-based computer, that have more enhanced graphic capabilities.
		HiDef,
	};

	//Specifies the buffer to use when calling Clear.
	enum class ClearOptions {
		//A render target.
		Target,
		//A depth buffer.
		DepthBuffer,
		//A stencil buffer.
		Stencil,
	};

	//Defines flags that describe the relationship between the adapter refresh rate and the rate at which Present operations are completed. 
	enum class PresentInterval {
		//Equivalent to setting One.
		Default,
		//The driver waits for the vertical retrace period (the runtime will beam trace to prevent tearing).
		//Present operations are not affected more frequently than the screen refresh rate; 
		//the runtime completes one Present operation per adapter refresh period, at most.
		//This option is always available for both windowed and full-screen swap chains.
		One,
		//The driver waits for the vertical retrace period.
		//Present operations are not affected more frequently than every second screen refresh. 
		Two,
		//The runtime updates the window client area immediately, and might do so more than once during the adapter refresh period.
		//Present operations might be affected immediately.
		//This option is always available for both windowed and full-screen swap chains.
		Immediate
	};

	//Defines how vertex data is ordered.
	enum class PrimitiveType {
		//The data is ordered as a sequence of triangles; each triangle is described by three new vertices.
		//Back-face culling is affected by the current winding-order render state.
		TriangleList,
		//The data is ordered as a sequence of triangles; each triangle is described by two new vertices and one vertex from the previous triangle. 
		//The back-face culling flag is flipped automatically on even-numbered triangles.
		TriangleStrip,
		//The data is ordered as a sequence of line segments; each line segment is described by two new vertices.
		//The count may be any positive integer
		LineList,
		//The data is ordered as a sequence of line segments; each line segment is described by one new vertex and the last vertex from the previous line segment.
		//The count may be any positive integer.
		LineStrip,
	};	

	enum class RenderTargetUsage
	{
		DiscardContents,
		PreserveContents,
		PlatformContents,
	};

	enum class SaveStateMode {
		None,
		SaveState
	};

	enum class SetDataOptions
	{
		None = 0,
		Discard = 1,
		NoOverwrite = 2,
	};

	enum class SpriteEffects
	{
		None = 0,
		FlipHorizontally = 1,
		FlipVertically = 2,
	};

	enum class SpriteSortMode
	{
		Deferred,
		Immediate,
		Texture,
		BackToFront,
		FrontToBack,
	};

	//Defines stencil buffer operations.
	enum class StencilOperation
	{
		//Does not update the stencil-buffer entry. This is the default value.
		Keep,
		//Sets the stencil-buffer entry to 0.
		Zero,
		//Replaces the stencil-buffer entry with a reference value.
		Replace,
		//Increments the stencil-buffer entry, clamping to the maximum value.
		IncrementSaturation,
		//Decrements the stencil-buffer entry, clamping to 0.
		DecrementSaturation,
		//Inverts the bits in the stencil-buffer entry.
		Invert,
		//Increments the stencil-buffer entry, wrapping to 0 if the new value exceeds the maximum value.
		Increment,
		//Decrements the stencil-buffer entry, wrapping to the maximum value if the new value is less than 0.
		Decrement,
	};

	enum class SwapEffect {
		Discard,
		Sequential,
		FlipSequential,
		FlipDiscard
	};

	//Defines modes for addressing texels using texture coordinates that are outside of the typical range of 0.0 to 1.0.
	enum class TextureAddressMode {
		//Tile the texture at every integer junction.
		//For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed. 
		Wrap,
		//Similar to Wrap, except that the texture is flipped at every integer junction. 
		//For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again, and so on.
		Mirror,
		//Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
		Clamp,
		//Texture coordinates outside the range [0.0, 1.0] are set to the border color specified.
		Border,
		//Similar to Mirror and Clamp.
		//Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
		MirrorOnce
	};

	//Defines filtering types during texture sampling. 
	enum class TextureFilter {
		//Use linear filtering.
		Linear,
		//Use point filtering.
		Point,
		//Use anisotropic filtering.
		Anisotropic,
		//Use linear filtering to shrink or expand, and point filtering between mipmap levels (mip).
		LinearMipPoint,
		//Use point filtering to shrink (minify) or expand (magnify), and linear filtering between mipmap levels.
		PointMipLinear,
		//Use linear filtering to shrink, point filtering to expand, and linear filtering between mipmap levels.
		MinLinearMagPointMipLinear,
		//Use linear filtering to shrink, point filtering to expand, and point filtering between mipmap levels.
		MinLinearMagPointMipPoint,
		//Use point filtering to shrink, linear filtering to expand, and linear filtering between mipmap levels.
		MinPointMagLinearMipLinear,
		//Use point filtering to shrink, linear filtering to expand, and point filtering between mipmap levels.
		MinPointMagLinearMipPoint,
	};

	//Defines vertex element formats.
	enum class VertexElementFormat
	{
		//Single - component, 32 - bit floating - point, expanded to(float, 0, 0, 1).
		Single,
		//Two-component, 32-bit floating-point, expanded to (float, Float32 value, 0, 1).
		Vector2,
		//Three-component, 32-bit floating point, expanded to (float, float, float, 1).
		Vector3,
		//Four-component, 32-bit floating point, expanded to (float, float, float, float).
		Vector4,
		//Four-component, packed, unsigned byte, mapped to 0 to 1 range. Input is in Int32 format (ARGB) expanded to (R, G, B, A).
		Color,
		//Four-component, unsigned byte.
		Byte4,
		//Two-component, signed short expanded to (value, value, 0, 1).
		Short2,
		//Four-component, signed short expanded to (value, value, value, value).
		Short4,
		//Normalized, two-component, signed short, expanded to (first short/32767.0, second short/32767.0, 0, 1). This type is valid for vertex shader version 2.0 or higher.
		NormalizedShort2,
		//Normalized, four-component, signed short, expanded to (first short/32767.0, second short/32767.0, third short/32767.0, fourth short/32767.0). This type is valid for vertex shader version 2.0 or higher.
		NormalizedShort4,
		//Two-component, 16-bit floating point expanded to (value, value, value, value). This type is valid for vertex shader version 2.0 or higher.
		HalfVector2,
		//Four-component, 16-bit floating-point expanded to (value, value, value, value). This type is valid for vertex shader version 2.0 or higher.
		HalfVector4,
	};

	//Defines usage for vertex elements.
	enum class VertexElementUsage
	{
		//Position data. (Position with UsageIndex = 0 ) specifies the nontransformed position in fixed-function vertex processing and the N-patch tessellator. (Position with UsageIndex = 1) specifies the nontransformed position in the fixed-function vertex shader for skinning.
		Position,
		//Vertex data contains diffuse or specular color. (Color with UsageIndex = 0) specifies the diffuse color in the fixed-function vertex shader and in pixel shaders prior to ps_3_0. (Color with UsageIndex = 1) specifies the specular color in the fixed-function vertex shader and in pixel shaders prior to ps_3_0.
		Color,
		//Texture coordinate data. (TextureCoordinate, n) specifies texture coordinates in fixed-function vertex processing and in pixel shaders prior to ps_3_0. These coordinates can be used to pass user-defined data.
		TextureCoordinate,
		//Vertex normal data. (Normal with UsageIndex = 0) specifies vertex normals for fixed-function vertex processing and the N-patch tessellator. (Normal with UsageIndex = 1) specifies vertex normals for fixed-function vertex processing for skinning.
		Normal,
		//Vertex binormal data.
		Binormal,
		//Vertex tangent data.
		Tangent,
		//Blending indices data. (BlendIndices with UsageIndex = 0) specifies matrix indices for fixed-function vertex processing using indexed paletted skinning.
		BlendIndices,
		//Blending weight data. (BlendWeight with UsageIndex = 0) specifies the blend weights in fixed-function vertex processing.
		BlendWeight,
		//Vertex data contains depth data.
		Depth,
		//Vertex data contains fog data. (Fog with UsageIndex = 0) specifies a fog blend value to use after pixel shading is finished. This flag applies to pixel shaders prior to version ps_3_0.
		Fog,
		//Point size data. (PointSize with UsageIndex = 0) specifies the point-size attribute used by the setup engine of the rasterizer to expand a point into a quad for the point-sprite functionality.
		PointSize,
		// 	Vertex data contains sampler data. (Sample with UsageIndex = 0) specifies the displacement value to look up.
		Sample,
		//Single, positive floating-point value. (TessellateFactor with UsageIndex = 0) specifies a tessellation factor used in the tessellation unit to control the rate of tessellation.
		TessellateFactor,
	};
	
	using CompareFunction = ComparisonFunction;

	inline static const int SURFACE_FORMAT_COUNT = 19;
	inline static const int DEFAULT_BACKBUFFER_WIDTH = 800;
	inline static const int DEFAULT_BACKBUFFER_HEIGHT = 480;
}

#endif