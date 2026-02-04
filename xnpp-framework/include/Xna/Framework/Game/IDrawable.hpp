#ifndef XNA_FRAMEWORK_GAME_IDRAWABLE_HPP
#define XNA_FRAMEWORK_GAME_IDRAWABLE_HPP

namespace Xna {
	class GameTime;

	//Defines the interface for a drawable game component.
	class IDrawable {
	public:
		virtual ~IDrawable() = 0;
		//Indicates whether IDrawable.Draw should be called in Game.Draw for this game component.
		virtual bool Visible() const = 0;
		//The order in which to draw this object relative to other objects. Objects with a lower value are drawn first.
		virtual int DrawOrder() const = 0; 
		//Draws the IDrawable.
		virtual void Draw(GameTime& gameTime) = 0;
	};
}

#endif