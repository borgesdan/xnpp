#include "Xna/Framework/Game/GameComponentCollection.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <algorithm>

namespace Xna {
	void GameComponentCollection::InsertItem(size_t index, std::shared_ptr<IGameComponent> const& item) {
		if (Collection<std::shared_ptr<IGameComponent>>::IndexOf(item) != -1)
			throw CSharp::ArgumentException("CannotAddSameComponentMultipleTimes");
		
		Collection<std::shared_ptr<IGameComponent>>::InsertItem(index, item);

		if (item == nullptr)
			return;

		OnComponentAdded(GameComponentCollectionEventArgs(item));
	}

	void GameComponentCollection::RemoveItem(size_t index) {
		auto gameComponent = (*this)[index];

		Collection<std::shared_ptr<IGameComponent>>::RemoveItem(index);

		if (gameComponent == nullptr)
			return;

		OnComponentRemoved(GameComponentCollectionEventArgs(gameComponent));
	}
	void GameComponentCollection::SetItem(size_t index, std::shared_ptr<IGameComponent> const& item) {
		throw CSharp::NotSupportedException("CannotSetItemsIntoGameComponentCollection");
	}

	void GameComponentCollection::ClearItems() {
		for(size_t index = 0; index < Count(); ++index)
			OnComponentRemoved(GameComponentCollectionEventArgs((*this)[index]));

		ClearItems();
	}

	void GameComponentCollection::OnComponentAdded(GameComponentCollectionEventArgs const& eventArgs) {
		impl->componentAdded.Invoke(this, eventArgs);
	}

	void GameComponentCollection::OnComponentRemoved(GameComponentCollectionEventArgs const& eventArgs) {
		impl->componentRemoved.Invoke(this, eventArgs);
	}
}