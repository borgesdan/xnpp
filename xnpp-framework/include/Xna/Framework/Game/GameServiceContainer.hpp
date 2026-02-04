#ifndef XNA_FRAMEWORK_GAME_GAMESERVICECONTAINTER_HPP
#define XNA_FRAMEWORK_GAME_GAMESERVICECONTAINTER_HPP

#include <optional>
#include <vector>
#include <memory>

namespace Xna {
	//A collection of game services.
	class GameServiceContainer {
	public:
		//Initializes a new instance of this class, which represents a collection of game services.
		GameServiceContainer() {
			items = std::make_shared<std::vector<std::shared_ptr<IGameServiceItem>>>();
		}

		//Adds a service to the GameServiceContainer.
		template <typename T>
		void AddService(T const& item) {
			auto ptr = std::make_shared<GameServiceItem<T>>(item);
			auto rptr = dynamic_pointer_cast<IGameServiceItem>(ptr);

			items->push_back(rptr);
		}

		template <class TBase, class TDerived>
		void AddService(TDerived const& item) {
			auto ptr = std::make_shared<TDerived>(item);
			auto ptrBase = dynamic_pointer_cast<TBase>(ptr);

			auto serviceItem = std::make_shared<GameServiceItem<TBase>>(ptrBase);			
			auto baseServiceItem = dynamic_pointer_cast<IGameServiceItem>(serviceItem);

			items->push_back(baseServiceItem);
		}

		//Removes the object providing a specified service.
		template <typename T>
		void RemoveService() {
			for (size_t i = 0; i < items->size(); ++i) {
				auto ptr = dynamic_pointer_cast<GameServiceItem<T>>(items[i]);

				if (ptr != nullptr)
				{
					items->erase(items->begin() + i);
					break;
				}
			}
		}

		template <typename T>
		std::shared_ptr<T> GetService() {
			for (size_t i = 0; i < items->size(); ++i) {
				auto ptr = dynamic_pointer_cast<GameServiceItem<T>>(items->at(i));

				if (ptr != nullptr)
				{
					auto obj = dynamic_pointer_cast<T>(ptr->item);

					if (obj != nullptr)
						return obj;
				}
			}

			return nullptr;
		}

	private:
		struct IGameServiceItem {
			virtual ~IGameServiceItem() = default;
		};

		template <typename T>
		struct GameServiceItem : public IGameServiceItem {
			GameServiceItem(T const& obj) {
				item = std::make_shared<T>(obj);
			}

			GameServiceItem(std::shared_ptr<T> const& ptr) : item(ptr) {

			}

			virtual ~GameServiceItem() override {}

			std::shared_ptr<T> item;
		};

	private:
		std::shared_ptr<std::vector<std::shared_ptr<IGameServiceItem>>> items;
	};
}

#endif