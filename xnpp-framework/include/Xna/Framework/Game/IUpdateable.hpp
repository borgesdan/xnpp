#ifndef XNA_FRAMEWORK_GAME_IUPDATABLE_HPP
#define XNA_FRAMEWORK_GAME_IUPDATABLE_HPP

namespace Xna {
	class GameTime;

	//Defines an interface for a game component that should be updated in Game.Update.
	class IUpdateable {
	public:
		virtual ~IUpdateable() = 0;
		//Indicates whether the game component's Update method should be called in Game.Update.
		virtual bool Enabled() const = 0;
		//Indicates when the game component should be updated relative to other game components. Lower values are updated first.
		virtual int UpdateOrder() const = 0;
		//Called when the game component should be updated.
		virtual void Update(GameTime& gameTime) = 0;
	};
}

#endif