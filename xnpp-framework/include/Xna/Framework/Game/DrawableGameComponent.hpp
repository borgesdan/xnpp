#ifndef XNA_FRAMEWORK_GAME_DRAWABLEGAMECOMPONENT_HPP
#define XNA_FRAMEWORK_GAME_DRAWABLEGAMECOMPONENT_HPP

#include "GameTime.hpp"
#include "IDrawable.hpp"
#include "GameComponent.hpp"
#include <cstdint>
#include <memory>

namespace Xna {
	//A game component that is notified when it needs to draw itself.
	class DrawableGameComponent : public GameComponent, public IDrawable {
	public:
		//Creates a new instance of DrawableGameComponent.
		DrawableGameComponent(Xna::Game const& game) : GameComponent(game) {
		}
		
		virtual ~DrawableGameComponent() override = default;

		//Order in which the component should be drawn, relative to other components that are in the same GameComponentCollection.
		inline int32_t DrawOrder() const { return drawOrder; }
		//Order in which the component should be drawn, relative to other components that are in the same GameComponentCollection.
		inline void DrawOrder(int32_t value) { drawOrder = value; }
		//Indicates whether Draw should be called.
		inline bool Visible() const { return visible; }
		//Indicates whether Draw should be called.
		inline void Visible(bool value) { visible = value; }

		//Called when the DrawableGameComponent needs to be drawn. Override this method with component-specific drawing code.
		virtual void Draw(GameTime& gameTime){}
		// 	Initializes the component. Override this method to load any non-graphics resources and query for any required services.
		virtual void Initialize() override {
			//TODO: GameComponent::Initialize()
			LoadContent();			
		}

	protected:
		//Called when graphics resources need to be loaded. Override this method to load any component-specific graphics resources.
		virtual void LoadContent(){}
		//Called when graphics resources need to be unloaded. Override this method to unload any component-specific graphics resources.
		virtual void UnloadContent(){}

	private:
		bool visible{ true };
		int32_t drawOrder{ 0 };
	};
}

#endif