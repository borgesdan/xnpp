#ifndef XNA_FRAMEWORK_GAME_GAMECOMPONENT_HPP
#define XNA_FRAMEWORK_GAME_GAMECOMPONENT_HPP

#include "IGameComponent.hpp"
#include "IUpdateable.hpp"
#include "Game.hpp"
#include "GameTime.hpp"
#include <cstdint>
#include <optional>
#include <memory>

namespace Xna {
	//A game component that is notified when it needs to draw itself.
	class GameComponent : public IGameComponent, public IUpdateable {
	public:		
		//Initializes a new instance of this class.
		GameComponent(Game const& game) {}

		virtual ~GameComponent() override = default;

		//Indicates whether GameComponent.Update should be called when Game.Update is called.
		bool Enabled() const override { return enabled; }
		//Indicates whether GameComponent.Update should be called when Game.Update is called.
		void Enabled(bool value) { enabled = value; }
		//Indicates the order in which the GameComponent should be updated relative to other GameComponent instances. Lower values are updated first.
		int32_t UpdateOrder() const override { return updateOrder; }
		//Indicates the order in which the GameComponent should be updated relative to other GameComponent instances. Lower values are updated first.
		void UpdateOrder(int32_t value) { updateOrder = value; }
		//Gets the Game associated with this GameComponent.
		std::optional<Xna::Game> Game() const { return game; }
		//Called when the GameComponent needs to be initialized.
		virtual void Initialize() override {}
		//Called when the GameComponent needs to be updated. Override this method with component-specific update code.
		virtual void Update(GameTime& gameTime) override {}

	private:
		bool enabled{ true };
		int32_t updateOrder{ 0 };
		std::optional<Xna::Game> game;
	};
}

#endif