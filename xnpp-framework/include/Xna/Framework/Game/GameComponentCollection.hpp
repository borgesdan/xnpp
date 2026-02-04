#ifndef XNA_FRAMEWORK_GAME_GAMECOMPONENTCOLLECTION_HPP
#define XNA_FRAMEWORK_GAME_GAMECOMPONENTCOLLECTION_HPP

#include <vector>
#include <memory>
#include "IGameComponent.hpp"
#include "Xna/CSharp/Event.hpp"
#include "Xna/CSharp/Collection.hpp"

namespace Xna {
	struct GameComponentCollectionEventArgs : public CSharp::EventArgs {
	public:
		inline std::shared_ptr<IGameComponent> GameComponent() const { return gameComponent; }

		inline GameComponentCollectionEventArgs(std::shared_ptr<IGameComponent> gameComponent):
			gameComponent(gameComponent){ }

	private:
		std::shared_ptr<IGameComponent> gameComponent;
	};

	//A collection of game components.
	class GameComponentCollection final : public CSharp::Collection<std::shared_ptr<IGameComponent>> {
	public:
		inline GameComponentCollection() {
			impl = std::make_shared<Implementation>();
		}

		~GameComponentCollection() override = default;

		inline CSharp::Event<GameComponentCollectionEventArgs> ComponentAdded() const { return impl->componentAdded; }
		inline CSharp::Event<GameComponentCollectionEventArgs> ComponentRemoved() const { return impl->componentRemoved; }		

		inline GameComponentCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(GameComponentCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		void InsertItem(size_t index, std::shared_ptr<IGameComponent> const& item);
		void RemoveItem(size_t index) override;
		void SetItem(size_t index, std::shared_ptr<IGameComponent> const& item);
		void ClearItems() override;

	private:
		void OnComponentAdded(GameComponentCollectionEventArgs const& eventArgs);
		void OnComponentRemoved(GameComponentCollectionEventArgs const& eventArgs);

		struct Implementation {
			CSharp::Event<GameComponentCollectionEventArgs> componentAdded;
			CSharp::Event<GameComponentCollectionEventArgs> componentRemoved;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif