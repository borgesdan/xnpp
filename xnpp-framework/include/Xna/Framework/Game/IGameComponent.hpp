#ifndef XNA_FRAMEWORK_GAME_IGAMECOMPONENT_HPP
#define XNA_FRAMEWORK_GAME_IGAMECOMPONENT_HPP

namespace Xna {
	//Defines an interface for game components.
	class IGameComponent {
	public:
		virtual ~IGameComponent() = 0;
		//Called when the component should be initialized. This method can be used for tasks like querying for services the component needs and setting up non-graphics resources.
		virtual void Initialize() = 0;
	};
}

#endif