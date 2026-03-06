#ifndef XNA_FRAMEWORK_GRAPHICS_SPRITEFONT_HPP
#define XNA_FRAMEWORK_GRAPHICS_SPRITEFONT_HPP

#include "_GraphicsDevice.hpp"
#include "Texture2D.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Internal/Macros.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include "SpriteBatch.hpp"
#include "Xna/Framework/Matrix.hpp"

namespace Xna {

	//Represents a font texture.
	class SpriteFont {
	public:
		// Returns the width and height of a string.
		inline Vector2 MeasureString(std::string_view text) const {
			return InternalMeasure(text);
		}

		// Returns the width and height of a string.
		inline Vector2 MeasureString(std::u32string_view text) const {
			return InternalMeasure(text);
		}

		//Gets or sets the default character for the font.
		inline std::optional<char32_t> DefaultCharacter() const {
			return impl->defaultCharacter;
		}

		//Gets or sets the default character for the font.
		inline void DefaultCharacter(std::optional<char32_t> value) {
			impl->defaultCharacter = value;
		}

		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		inline int32_t LineSpacing() const {
			return static_cast<int32_t>(impl->lineSpacing);
		}

		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		inline void LineSpacing(int32_t value) {
			impl->lineSpacing = static_cast<float>(value);
		}

		//Gets or sets the spacing of the font characters.
		inline float Spacing() const {
			return impl->spacing;
		}

		//Gets or sets the spacing of the font characters.
		inline void Spacing(float value) {
			impl->spacing = value;
		}

		inline const std::vector<char32_t>& Characters() const {
			return impl->characters;
		}

		XNPP_API SpriteFont(
			Texture2D const& texture,
			std::vector<Rectangle> const& glyphs,
			std::vector<Rectangle> const& cropping,
			std::vector<char32_t> const& charMap,
			int32_t lineSpacing,
			float spacing,
			std::vector<Vector3> const& kerning,
			std::optional<char32_t> const& defaultCharacter);

		XNPP_DECLARE_IMPL_WRAPPER(SpriteFont, impl);
	private:
		struct Implementation {
			Xna::Texture2D textureValue{ nullptr };
			std::vector<Rectangle> glyphData;
			std::vector<Rectangle> croppingData;
			std::vector<char32_t> characterMap;
			std::vector<Vector3> kerning;
			float lineSpacing{ 0 };
			float spacing{ 0 };
			std::optional<char32_t> defaultCharacter;
			std::vector<char32_t> characters;
		};		

		XNPP_API size_t GetIndexForCharacter(char32_t character) const;

		template <typename T> Vector2 InternalMeasure(T text) const {
			if (text.size() == 0)
				return {};

			Vector2 zero(0, impl->lineSpacing);

			auto val2 = 0.0f;
			auto num = 0;
			auto val1 = 0.0f;
			auto flag = true;

			for (size_t index = 0; index < text.size(); ++index) {
				if (text[index] != '\r') {
					if (text[index] == '\n') {
						zero.X += std::max(val1, 0.0f);
						val1 = 0.0f;
						val2 = std::max(zero.X, val2);
						zero.Y = impl->lineSpacing;
						flag = true;
						++num;
					}
					else
					{
						Vector3 vector3 = impl->kerning[GetIndexForCharacter(text[index])];
						if (flag)
							vector3.X = std::max(vector3.X, 0.0f);
						else
							zero.X += impl->spacing + val1;

						zero.X += vector3.X + vector3.Y;
						val1 = vector3.Z;

						const auto& rectangle = impl->croppingData[GetIndexForCharacter(text[index])];
						zero.Y = std::max(zero.Y, static_cast<float>(rectangle.Height));
						flag = false;
					}
				}
			}

			zero.X += std::max(val1, 0.0f);
			zero.Y += num * impl->lineSpacing;
			zero.X = std::max(zero.X, val2);
			return zero;
		}

		template <typename T> void InternalDraw(
			T text,
			SpriteBatch& spriteBatch,
			Vector2 textblockPosition,
			Color color,
			float rotation,
			Vector2 origin,
			Vector2 scale,
			SpriteEffects spriteEffects, float depth) const {

			auto result1 = Matrix::CreateRotationZ(rotation);
			auto result2 = Matrix::CreateTranslation(-origin.X * scale.X, -origin.Y * scale.Y, 0.0f);

			result1 = Matrix::Multiply(result2, result1);

			int32_t num1 = 1;
			float num2 = 0.0f;
			Vector2 vector2;
			bool flag = true;

			if (((int)spriteEffects & (int)SpriteEffects::FlipHorizontally) == (int)SpriteEffects::FlipHorizontally) {
				num2 = InternalMeasure(text).X * scale.X;
				num1 = -1;
			}

			vector2.Y = ((int)spriteEffects & (int)SpriteEffects::FlipVertically) != (int)SpriteEffects::FlipVertically
				? 0.0f : (InternalMeasure(text).Y - impl->lineSpacing) * scale.Y;

			vector2.X = num2;

			for (size_t index = 0; index < text.size(); ++index) {
				const auto character = text[index];
				switch (character)
				{
				case '\n':
					flag = true;
					vector2.X = num2;

					if (((int)spriteEffects & (int)SpriteEffects::FlipVertically) == (int)SpriteEffects::FlipVertically) {
						vector2.Y -= impl->lineSpacing * scale.Y;
						continue;
					}

					vector2.Y += impl->lineSpacing * scale.Y;
					continue;
				case '\r':
					continue;
				default:
					const auto indexForCharacter = GetIndexForCharacter(character);
					auto& vector3 = impl->kerning[indexForCharacter];

					if (flag)
						vector3.X = std::max(vector3.X, 0.0f);
					else
						vector2.X += impl->spacing * scale.X * num1;

					vector2.X += vector3.X * scale.X * num1;
					auto& rectangle1 = impl->glyphData[indexForCharacter];
					auto& rectangle2 = impl->croppingData[indexForCharacter];

					if (((int)spriteEffects & (int)SpriteEffects::FlipVertically) == (int)SpriteEffects::FlipVertically)
						rectangle2.Y = impl->lineSpacing - rectangle1.Height - rectangle2.Y;

					if (((int)spriteEffects & (int)SpriteEffects::FlipHorizontally) == (int)SpriteEffects::FlipHorizontally)
						rectangle2.X -= rectangle2.Width;

					auto result3 = vector2;
					result3.X += rectangle2.X * scale.X;
					result3.Y += rectangle2.Y * scale.Y;

					result3 = Vector2::Transform(result3, result1);

					result3 = result3 + textblockPosition;

					spriteBatch.Draw(impl->textureValue, result3, color, rectangle1, rotation, Vector2::Zero(), scale, spriteEffects, depth);

					flag = false;
					vector2.X += (vector3.Y + vector3.Z) * scale.X * (float)num1;
					continue;
				}
			}
		}

		std::shared_ptr<Implementation> impl;

		friend class SpriteBatch;
	};
}

#endif