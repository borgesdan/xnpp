#ifndef XNA_FRAMEWORK_COLOR_HPP
#define XNA_FRAMEWORK_COLOR_HPP

#include "IPackedVector.hpp"
#include "PackUtils.hpp"
#include "MathHelper.hpp"
#include <optional>

namespace Xna {
	//Represents a four-component color using red, green, blue, and alpha data. 
	//[!] This struct does not inherit from IPackedVector<uint> or IPackedVector to allow its functions to be constexpr. Part of the implementation is based on MonoGame.
	struct Color final {
		//Creates a new instance of the class.
		constexpr Color() = default;

		//Creates a new instance of the class.
		constexpr Color(uint32_t packedValue)
			: _packedValue(packedValue) {
		}

		//Creates a new instance of the class.
		constexpr Color(int32_t r, int32_t g, int32_t b, int32_t a = 255U) {
			if (((r | g | b | a) & -256) != 0)
			{
				r = ClampToByte32(r);
				g = ClampToByte32(g);
				b = ClampToByte32(b);
				a = ClampToByte32(a);
			}

			g <<= 8;
			b <<= 16;
			a <<= 24;

			_packedValue = static_cast<uint32_t>(r | g | b | a);
		}

		//Creates a new instance of the class.
		constexpr Color(float r, float g, float b, float a = 1.0F)
			: Color(static_cast<int32_t>(r * 255), static_cast<int32_t>(g * 255), static_cast<int32_t>(b * 255), static_cast<int32_t>(a * 255)) {
		}

		//Creates a new instance of the class.
		constexpr Color(Vector3 const& vector)
			: Color(static_cast<int32_t>(vector.X * 255), static_cast<int32_t>(vector.Y * 255), static_cast<int32_t>(vector.Z * 255)) {
		}

		//Creates a new instance of the class.
		constexpr Color(Vector4 const& vector)
			: Color(static_cast<int32_t>(vector.X * 255), static_cast<int32_t>(vector.Y * 255), static_cast<int32_t>(vector.Z * 255), static_cast<int32_t>(vector.W * 255)) {
		}

		//[!]Function removed
		//void PackFromVector4(Vector4 const& vector);

		//Converts a non-premultipled alpha color to a color that contains premultiplied alpha.
		static constexpr  Color FromNonPremultiplied(Vector4 const& vector) {
			return Color(vector.X * vector.W, vector.Y * vector.W, vector.Z * vector.W, vector.W);
		}

		//Converts a non-premultipled alpha color to a color that contains premultiplied alpha.
		static constexpr Color FromNonPremultiplied(int32_t r, int32_t g, int32_t b, int32_t a) {
			return Color(r * a / 255, g * a / 255, b * a / 255, a);
		}

		//Gets a three-component vector representation for this object.
		constexpr Vector3 ToVector3() const {
			Vector3 vector3;
			const auto byteMax = static_cast<uint32_t>(ByteMaxValue);
			vector3.X = PackUtils::UnpackUNorm(byteMax, _packedValue);
			vector3.Y = PackUtils::UnpackUNorm(byteMax, _packedValue >> 8);
			vector3.Z = PackUtils::UnpackUNorm(byteMax, _packedValue >> 16);
			return vector3;
		}

		//Gets a four-component vector representation for this object.
		constexpr Vector4 ToVector4() const {
			Vector4 vector4;
			const auto byteMax = static_cast<uint32_t>(ByteMaxValue);
			vector4.X = PackUtils::UnpackUNorm(byteMax, _packedValue);
			vector4.Y = PackUtils::UnpackUNorm(byteMax, _packedValue >> 8);
			vector4.Z = PackUtils::UnpackUNorm(byteMax, _packedValue >> 16);
			vector4.W = PackUtils::UnpackUNorm(byteMax, _packedValue >> 24);
			return vector4;
		}

		//Gets or sets the red component value of this Color. 
		constexpr uint8_t R() const {
			return static_cast<uint8_t>(_packedValue);
		}

		//Gets or sets the red component value of this Color. 
		constexpr void R(uint8_t value) {
			_packedValue = _packedValue & (UintMaxValue - 255) | static_cast<uint32_t>(value);
		}

		//Gets or sets the green component value of this Color. 
		constexpr uint8_t G() const {
			return static_cast<uint8_t>(_packedValue >> 8);
		}

		//Gets or sets the green component value of this Color. 
		constexpr void G(uint8_t value) {
			_packedValue = (static_cast<int32_t>(_packedValue) & -65281 | static_cast<int32_t>(value) << 8);
		}

		//Gets or sets the blue component value of this Color. 
		constexpr uint8_t B() const {
			return static_cast<uint8_t>(_packedValue >> 16);
		}

		//Gets or sets the blue component value of this Color. 
		constexpr void B(uint8_t value) {
			_packedValue = (static_cast<int32_t>(_packedValue) & -16711681 | static_cast<int32_t>(value) << 16);
		}

		//Gets or sets the alpha component value. 
		constexpr uint8_t A() const {
			return static_cast<uint8_t>(_packedValue >> 24);
		}

		//Gets or sets the alpha component value. 
		constexpr void A(uint8_t value) {
			_packedValue = (static_cast<int32_t>(_packedValue) & 16777215 | static_cast<int32_t>(value) << 24);
		}

		//Gets or sets the current color as a packed value.
		constexpr uint32_t PackedValue() const {
			return _packedValue;
		}

		//Gets or sets the current color as a packed value.
		constexpr void PackedValue(uint32_t const& value) {
			_packedValue = value;
		}

		//Linearly interpolate a color.
		static Color Lerp(Color const& value1, Color const& value2, float amount) {
			const auto _amount = MathHelper::Clamp(amount, 0.0f, 1.0f);
			return Color(
				static_cast<int32_t>(MathHelper::Lerp(value1.R(), value2.R(), amount)),
				static_cast<int32_t>(MathHelper::Lerp(value1.G(), value2.G(), amount)),
				static_cast<int32_t>(MathHelper::Lerp(value1.B(), value2.B(), amount)),
				static_cast<int32_t>(MathHelper::Lerp(value1.A(), value2.A(), amount)));
		}

		//Multiply each color component by the scale factor.
		static constexpr Color Multiply(Color const& value, float scale) {
			const uint32_t r = value.R();
			const uint32_t g = value.G();
			const uint32_t b = value.B();
			const uint32_t a = value.A();

			scale *= 65536.0f;

			const uint32_t num5 = scale >= 0.0F ? (scale <= 16777215.0F ? static_cast<uint32_t>(scale) : 16777215U) : 0U;

			uint32_t r1 = r * num5 >> 16;
			uint32_t g1 = g * num5 >> 16;
			uint32_t b1 = b * num5 >> 16;
			uint32_t a1 = a * num5 >> 16;

			if (r1 > ByteMaxValue)
				r1 = ByteMaxValue;

			if (g1 > ByteMaxValue)
				g1 = ByteMaxValue;

			if (b1 > ByteMaxValue)
				b1 = ByteMaxValue;

			if (a1 > ByteMaxValue)
				a1 = ByteMaxValue;

			const auto r2 = static_cast<int32_t>(r1);
			const auto g2 = static_cast<int32_t>(g1);
			const auto b2 = static_cast<int32_t>(b1);
			const auto a2 = static_cast<int32_t>(a1);

			Color color;
			color._packedValue = static_cast<uint32_t>(r2 | g2 << 8 | b2 << 16 | a2 << 24);
			return color;
		}

		constexpr bool operator==(Color const& other) const {
			return _packedValue == other._packedValue;
		}

		friend constexpr Color operator*(Color const& value, float const& scale) {
			return Color::Multiply(value, scale);
		}

		constexpr operator uint32_t() const {
			return _packedValue;
		}

		constexpr operator std::optional<Color>() const {
			return std::make_optional<Color>(_packedValue);
		}

	private:
		uint32_t _packedValue{ 0 };

		static constexpr int32_t ClampToByte32(int32_t value) noexcept {
			if (value < 0)
				return 0;

			return value > ByteMaxValue ? ByteMaxValue : value;
		}

		//[!]Function removed
		//static uint32_t PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW);

		static constexpr uint8_t ByteMaxValue = (std::numeric_limits<uint8_t>::max)();
		static constexpr uint32_t UintMaxValue = (std::numeric_limits<uint32_t>::max)();

	public:
		static constexpr Color Transparent() { return Color(0U); }
		static constexpr Color AliceBlue() { return Color(4294965488U); }
		static constexpr Color AntiqueWhite() { return Color(4292340730U); }
		static constexpr Color Aqua() { return Color(4294967040U); }
		static constexpr Color Aquamarine() { return Color(4292149119U); }
		static constexpr Color Azure() { return Color(4294967280U); }
		static constexpr Color Beige() { return Color(4292670965U); }
		static constexpr Color Bisque() { return Color(4291093759U); }
		static constexpr Color Black() { return Color(4278190080U); }
		static constexpr Color BlanchedAlmond() { return Color(4291685375U); }
		static constexpr Color Blue() { return Color(4294901760U); }
		static constexpr Color BlueViolet() { return Color(4293012362U); }
		static constexpr Color Brown() { return Color(4280953509U); }
		static constexpr Color BurlyWood() { return Color(4287084766U); }
		static constexpr Color CadetBlue() { return Color(4288716383U); }
		static constexpr Color Chartreuse() { return Color(4278255487U); }
		static constexpr Color Chocolate() { return Color(4280183250U); }
		static constexpr Color Coral() { return Color(4283465727U); }
		static constexpr Color CornflowerBlue() { return Color(4293760356U); }
		static constexpr Color Cornsilk() { return Color(4292671743U); }
		static constexpr Color Crimson() { return Color(4282127580U); }
		static constexpr Color Cyan() { return Color(4294967040U); }
		static constexpr Color DarkBlue() { return Color(4287299584U); }
		static constexpr Color DarkCyan() { return Color(4287335168U); }
		static constexpr Color DarkGoldenrod() { return Color(4278945464U); }
		static constexpr Color DarkGray() { return Color(4289309097U); }
		static constexpr Color DarkGreen() { return Color(4278215680U); }
		static constexpr Color DarkKhaki() { return Color(4285249469U); }
		static constexpr Color DarkMagenta() { return Color(4287299723U); }
		static constexpr Color DarkOliveGreen() { return Color(4281297749U); }
		static constexpr Color DarkOrange() { return Color(4278226175U); }
		static constexpr Color DarkOrchid() { return Color(4291572377U); }
		static constexpr Color DarkRed() { return Color(4278190219U); }
		static constexpr Color DarkSalmon() { return Color(4286224105U); }
		static constexpr Color DarkSeaGreen() { return Color(4287347855U); }
		static constexpr Color DarkSlateBlue() { return Color(4287315272U); }
		static constexpr Color DarkSlateGray() { return Color(4283387695U); }
		static constexpr Color DarkTurquoise() { return Color(4291939840U); }
		static constexpr Color DarkViolet() { return Color(4292018324U); }
		static constexpr Color DeepPink() { return Color(4287829247U); }
		static constexpr Color DeepSkyBlue() { return Color(4294950656U); }
		static constexpr Color DimGray() { return Color(4285098345U); }
		static constexpr Color DodgerBlue() { return Color(4294938654U); }
		static constexpr Color Firebrick() { return Color(4280427186U); }
		static constexpr Color FloralWhite() { return Color(4293982975U); }
		static constexpr Color ForestGreen() { return Color(4280453922U); }
		static constexpr Color Fuchsia() { return Color(4294902015U); }
		static constexpr Color Gainsboro() { return Color(4292664540U); }
		static constexpr Color GhostWhite() { return Color(4294965496U); }
		static constexpr Color Gold() { return Color(4278245375U); }
		static constexpr Color Goldenrod() { return Color(4280329690U); }
		static constexpr Color Gray() { return Color(4286611584U); }
		static constexpr Color Green() { return Color(4278222848U); }
		static constexpr Color GreenYellow() { return Color(4281335725U); }
		static constexpr Color Honeydew() { return Color(4293984240U); }
		static constexpr Color HotPink() { return Color(4290013695U); }
		static constexpr Color IndianRed() { return Color(4284243149U); }
		static constexpr Color Indigo() { return Color(4286709835U); }
		static constexpr Color Ivory() { return Color(4293984255U); }
		static constexpr Color Khaki() { return Color(4287424240U); }
		static constexpr Color Lavender() { return Color(4294633190U); }
		static constexpr Color LavenderBlush() { return Color(4294308095U); }
		static constexpr Color LawnGreen() { return Color(4278254716U); }
		static constexpr Color LemonChiffon() { return Color(4291689215U); }
		static constexpr Color LightBlue() { return Color(4293318829U); }
		static constexpr Color LightCoral() { return Color(4286611696U); }
		static constexpr Color LightCyan() { return Color(4294967264U); }
		static constexpr Color LightGoldenrodYellow() { return Color(4292016890U); }
		static constexpr Color LightGreen() { return Color(4287688336U); }
		static constexpr Color LightGray() { return Color(4292072403U); }
		static constexpr Color LightPink() { return Color(4290885375U); }
		static constexpr Color LightSalmon() { return Color(4286226687U); }
		static constexpr Color LightSeaGreen() { return Color(4289376800U); }
		static constexpr Color LightSkyBlue() { return Color(4294626951U); }
		static constexpr Color LightSlateGray() { return Color(4288252023U); }
		static constexpr Color LightSteelBlue() { return Color(4292789424U); }
		static constexpr Color LightYellow() { return Color(4292935679U); }
		static constexpr Color Lime() { return Color(4278255360U); }
		static constexpr Color LimeGreen() { return Color(4281519410U); }
		static constexpr Color Linen() { return Color(4293325050U); }
		static constexpr Color Magenta() { return Color(4294902015U); }
		static constexpr Color Maroon() { return Color(4278190208U); }
		static constexpr Color MediumAquamarine() { return Color(4289383782U); }
		static constexpr Color MediumBlue() { return Color(4291624960U); }
		static constexpr Color MediumOrchid() { return Color(4292040122U); }
		static constexpr Color MediumPurple() { return Color(4292571283U); }
		static constexpr Color MediumSeaGreen() { return Color(4285641532U); }
		static constexpr Color MediumSlateBlue() { return Color(4293814395U); }
		static constexpr Color MediumSpringGreen() { return Color(4288346624U); }
		static constexpr Color MediumTurquoise() { return Color(4291613000U); }
		static constexpr Color MediumVioletRed() { return Color(4286911943U); }
		static constexpr Color MidnightBlue() { return Color(4285536537U); }
		static constexpr Color MintCream() { return Color(4294639605U); }
		static constexpr Color MistyRose() { return Color(4292994303U); }
		static constexpr Color Moccasin() { return Color(4290110719U); }
		static constexpr Color NavajoWhite() { return Color(4289584895U); }
		static constexpr Color Navy() { return Color(4286578688U); }
		static constexpr Color OldLace() { return Color(4293326333U); }
		static constexpr Color Olive() { return Color(4278222976U); }
		static constexpr Color OliveDrab() { return Color(4280520299U); }
		static constexpr Color Orange() { return Color(4278232575U); }
		static constexpr Color OrangeRed() { return Color(4278207999U); }
		static constexpr Color Orchid() { return Color(4292243674U); }
		static constexpr Color PaleGoldenrod() { return Color(4289390830U); }
		static constexpr Color PaleGreen() { return Color(4288215960U); }
		static constexpr Color PaleTurquoise() { return Color(4293848751U); }
		static constexpr Color PaleVioletRed() { return Color(4287852763U); }
		static constexpr Color PapayaWhip() { return Color(4292210687U); }
		static constexpr Color PeachPuff() { return Color(4290370303U); }
		static constexpr Color Peru() { return Color(4282353101U); }
		static constexpr Color Pink() { return Color(4291543295U); }
		static constexpr Color Plum() { return Color(4292714717U); }
		static constexpr Color PowderBlue() { return Color(4293320880U); }
		static constexpr Color Purple() { return Color(4286578816U); }
		static constexpr Color Red() { return Color(4278190335U); }
		static constexpr Color RosyBrown() { return Color(4287598524U); }
		static constexpr Color RoyalBlue() { return Color(4292962625U); }
		static constexpr Color SaddleBrown() { return Color(4279453067U); }
		static constexpr Color Salmon() { return Color(4285694202U); }
		static constexpr Color SandyBrown() { return Color(4284523764U); }
		static constexpr Color SeaGreen() { return Color(4283927342U); }
		static constexpr Color SeaShell() { return Color(4293850623U); }
		static constexpr Color Sienna() { return Color(4281160352U); }
		static constexpr Color Silver() { return Color(4290822336U); }
		static constexpr Color SkyBlue() { return Color(4293643911U); }
		static constexpr Color SlateBlue() { return Color(4291648106U); }
		static constexpr Color SlateGray() { return Color(4287660144U); }
		static constexpr Color Snow() { return Color(4294638335U); }
		static constexpr Color SpringGreen() { return Color(4286578432U); }
		static constexpr Color SteelBlue() { return Color(4290019910U); }
		static constexpr Color Tan() { return Color(4287411410U); }
		static constexpr Color Teal() { return Color(4286611456U); }
		static constexpr Color Thistle() { return Color(4292394968U); }
		static constexpr Color Tomato() { return Color(4282868735U); }
		static constexpr Color Turquoise() { return Color(4291878976U); }
		static constexpr Color Violet() { return Color(4293821166U); }
		static constexpr Color Wheat() { return Color(4289978101U); }
		static constexpr Color White() { return Color((std::numeric_limits<uint32_t>::max)()); }
		static constexpr Color WhiteSmoke() { return Color(4294309365U); }
		static constexpr Color Yellow() { return Color(4278255615U); }
		static constexpr Color YellowGreen() { return Color(4281519514U); }
	};

	struct Colors final {
		Colors() = delete;
		Colors(Colors&) = delete;
		Colors(Colors&&) = delete;

		static constexpr Color Transparent{ Color(0U) };
		static constexpr Color AliceBlue{ Color(4294965488U) };
		static constexpr Color AntiqueWhite{ Color(4292340730U) };
		static constexpr Color Aqua{ Color(4294967040U) };
		static constexpr Color Aquamarine{ Color(4292149119U) };
		static constexpr Color Azure{ Color(4294967280U) };
		static constexpr Color Beige{ Color(4292670965U) };
		static constexpr Color Bisque{ Color(4291093759U) };
		static constexpr Color Black{ Color(4278190080U) };
		static constexpr Color BlanchedAlmond{ Color(4291685375U) };
		static constexpr Color Blue{ Color(4294901760U) };
		static constexpr Color BlueViolet{ Color(4293012362U) };
		static constexpr Color Brown{ Color(4280953509U) };
		static constexpr Color BurlyWood{ Color(4287084766U) };
		static constexpr Color CadetBlue{ Color(4288716383U) };
		static constexpr Color Chartreuse{ Color(4278255487U) };
		static constexpr Color Chocolate{ Color(4280183250U) };
		static constexpr Color Coral{ Color(4283465727U) };
		static constexpr Color CornflowerBlue{ Color(4293760356U) };
		static constexpr Color Cornsilk{ Color(4292671743U) };
		static constexpr Color Crimson{ Color(4282127580U) };
		static constexpr Color Cyan{ Color(4294967040U) };
		static constexpr Color DarkBlue{ Color(4287299584U) };
		static constexpr Color DarkCyan{ Color(4287335168U) };
		static constexpr Color DarkGoldenrod{ Color(4278945464U) };
		static constexpr Color DarkGray{ Color(4289309097U) };
		static constexpr Color DarkGreen{ Color(4278215680U) };
		static constexpr Color DarkKhaki{ Color(4285249469U) };
		static constexpr Color DarkMagenta{ Color(4287299723U) };
		static constexpr Color DarkOliveGreen{ Color(4281297749U) };
		static constexpr Color DarkOrange{ Color(4278226175U) };
		static constexpr Color DarkOrchid{ Color(4291572377U) };
		static constexpr Color DarkRed{ Color(4278190219U) };
		static constexpr Color DarkSalmon{ Color(4286224105U) };
		static constexpr Color DarkSeaGreen{ Color(4287347855U) };
		static constexpr Color DarkSlateBlue{ Color(4287315272U) };
		static constexpr Color DarkSlateGray{ Color(4283387695U) };
		static constexpr Color DarkTurquoise{ Color(4291939840U) };
		static constexpr Color DarkViolet{ Color(4292018324U) };
		static constexpr Color DeepPink{ Color(4287829247U) };
		static constexpr Color DeepSkyBlue{ Color(4294950656U) };
		static constexpr Color DimGray{ Color(4285098345U) };
		static constexpr Color DodgerBlue{ Color(4294938654U) };
		static constexpr Color Firebrick{ Color(4280427186U) };
		static constexpr Color FloralWhite{ Color(4293982975U) };
		static constexpr Color ForestGreen{ Color(4280453922U) };
		static constexpr Color Fuchsia{ Color(4294902015U) };
		static constexpr Color Gainsboro{ Color(4292664540U) };
		static constexpr Color GhostWhite{ Color(4294965496U) };
		static constexpr Color Gold{ Color(4278245375U) };
		static constexpr Color Goldenrod{ Color(4280329690U) };
		static constexpr Color Gray{ Color(4286611584U) };
		static constexpr Color Green{ Color(4278222848U) };
		static constexpr Color GreenYellow{ Color(4281335725U) };
		static constexpr Color Honeydew{ Color(4293984240U) };
		static constexpr Color HotPink{ Color(4290013695U) };
		static constexpr Color IndianRed{ Color(4284243149U) };
		static constexpr Color Indigo{ Color(4286709835U) };
		static constexpr Color Ivory{ Color(4293984255U) };
		static constexpr Color Khaki{ Color(4287424240U) };
		static constexpr Color Lavender{ Color(4294633190U) };
		static constexpr Color LavenderBlush{ Color(4294308095U) };
		static constexpr Color LawnGreen{ Color(4278254716U) };
		static constexpr Color LemonChiffon{ Color(4291689215U) };
		static constexpr Color LightBlue{ Color(4293318829U) };
		static constexpr Color LightCoral{ Color(4286611696U) };
		static constexpr Color LightCyan{ Color(4294967264U) };
		static constexpr Color LightGoldenrodYellow{ Color(4292016890U) };
		static constexpr Color LightGreen{ Color(4287688336U) };
		static constexpr Color LightGray{ Color(4292072403U) };
		static constexpr Color LightPink{ Color(4290885375U) };
		static constexpr Color LightSalmon{ Color(4286226687U) };
		static constexpr Color LightSeaGreen{ Color(4289376800U) };
		static constexpr Color LightSkyBlue{ Color(4294626951U) };
		static constexpr Color LightSlateGray{ Color(4288252023U) };
		static constexpr Color LightSteelBlue{ Color(4292789424U) };
		static constexpr Color LightYellow{ Color(4292935679U) };
		static constexpr Color Lime{ Color(4278255360U) };
		static constexpr Color LimeGreen{ Color(4281519410U) };
		static constexpr Color Linen{ Color(4293325050U) };
		static constexpr Color Magenta{ Color(4294902015U) };
		static constexpr Color Maroon{ Color(4278190208U) };
		static constexpr Color MediumAquamarine{ Color(4289383782U) };
		static constexpr Color MediumBlue{ Color(4291624960U) };
		static constexpr Color MediumOrchid{ Color(4292040122U) };
		static constexpr Color MediumPurple{ Color(4292571283U) };
		static constexpr Color MediumSeaGreen{ Color(4285641532U) };
		static constexpr Color MediumSlateBlue{ Color(4293814395U) };
		static constexpr Color MediumSpringGreen{ Color(4288346624U) };
		static constexpr Color MediumTurquoise{ Color(4291613000U) };
		static constexpr Color MediumVioletRed{ Color(4286911943U) };
		static constexpr Color MidnightBlue{ Color(4285536537U) };
		static constexpr Color MintCream{ Color(4294639605U) };
		static constexpr Color MistyRose{ Color(4292994303U) };
		static constexpr Color Moccasin{ Color(4290110719U) };
		static constexpr Color NavajoWhite{ Color(4289584895U) };
		static constexpr Color Navy{ Color(4286578688U) };
		static constexpr Color OldLace{ Color(4293326333U) };
		static constexpr Color Olive{ Color(4278222976U) };
		static constexpr Color OliveDrab{ Color(4280520299U) };
		static constexpr Color Orange{ Color(4278232575U) };
		static constexpr Color OrangeRed{ Color(4278207999U) };
		static constexpr Color Orchid{ Color(4292243674U) };
		static constexpr Color PaleGoldenrod{ Color(4289390830U) };
		static constexpr Color PaleGreen{ Color(4288215960U) };
		static constexpr Color PaleTurquoise{ Color(4293848751U) };
		static constexpr Color PaleVioletRed{ Color(4287852763U) };
		static constexpr Color PapayaWhip{ Color(4292210687U) };
		static constexpr Color PeachPuff{ Color(4290370303U) };
		static constexpr Color Peru{ Color(4282353101U) };
		static constexpr Color Pink{ Color(4291543295U) };
		static constexpr Color Plum{ Color(4292714717U) };
		static constexpr Color PowderBlue{ Color(4293320880U) };
		static constexpr Color Purple{ Color(4286578816U) };
		static constexpr Color Red{ Color(4278190335U) };
		static constexpr Color RosyBrown{ Color(4287598524U) };
		static constexpr Color RoyalBlue{ Color(4292962625U) };
		static constexpr Color SaddleBrown{ Color(4279453067U) };
		static constexpr Color Salmon{ Color(4285694202U) };
		static constexpr Color SandyBrown{ Color(4284523764U) };
		static constexpr Color SeaGreen{ Color(4283927342U) };
		static constexpr Color SeaShell{ Color(4293850623U) };
		static constexpr Color Sienna{ Color(4281160352U) };
		static constexpr Color Silver{ Color(4290822336U) };
		static constexpr Color SkyBlue{ Color(4293643911U) };
		static constexpr Color SlateBlue{ Color(4291648106U) };
		static constexpr Color SlateGray{ Color(4287660144U) };
		static constexpr Color Snow{ Color(4294638335U) };
		static constexpr Color SpringGreen{ Color(4286578432U) };
		static constexpr Color SteelBlue{ Color(4290019910U) };
		static constexpr Color Tan{ Color(4287411410U) };
		static constexpr Color Teal{ Color(4286611456U) };
		static constexpr Color Thistle{ Color(4292394968U) };
		static constexpr Color Tomato{ Color(4282868735U) };
		static constexpr Color Turquoise{ Color(4291878976U) };
		static constexpr Color Violet{ Color(4293821166U) };
		static constexpr Color Wheat{ Color(4289978101U) };
		static constexpr Color White{ Color((std::numeric_limits<uint32_t>::max)()) };
		static constexpr Color WhiteSmoke{ Color(4294309365U) };
		static constexpr Color Yellow{ Color(4278255615U) };
		static constexpr Color YellowGreen{ Color(4281519514U) };	
	};
}

#endif