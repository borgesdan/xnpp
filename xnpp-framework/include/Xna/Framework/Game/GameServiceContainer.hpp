#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Xna {
	// A collection of game services.
	class GameServiceContainer final {
	public:
		constexpr GameServiceContainer() noexcept = default;
		~GameServiceContainer() = default;

		GameServiceContainer(const GameServiceContainer&) = delete;
		GameServiceContainer& operator=(const GameServiceContainer&) = delete;
		GameServiceContainer(GameServiceContainer&&) noexcept = default;
		GameServiceContainer& operator=(GameServiceContainer&&) noexcept = default;

		// 1. Transfere a posse de um std::unique_ptr<T> para o container
		template <typename T>
		void AddService(std::unique_ptr<T> item) {
			if (!item)
				throw std::invalid_argument("Service pointer cannot be null.");

			if (ContainsService<T>())
				throw std::invalid_argument("A service of this type is already registered.");

			items.push_back(std::make_unique<GameServiceItem<T>>(std::move(item)));
		}

		// 2. Transfere a posse de um std::unique_ptr<TDerived> registrado como TBase
		template <typename TBase, std::derived_from<TBase> TDerived>
		void AddService(std::unique_ptr<TDerived> item) {
			if (!item)
				throw std::invalid_argument("Service pointer cannot be null.");

			if (ContainsService<TBase>())
				throw std::invalid_argument("A service of this type is already registered.");

			std::unique_ptr<TBase> basePtr = std::move(item);
			items.push_back(std::make_unique<GameServiceItem<TBase>>(std::move(basePtr)));
		}

		// 3. Registra um ponteiro cru nao-proprietario (o ciclo de vida pertence a quem chamou, ex: Game)
		template <typename T>
		void AddService(T* item) {
			if (!item)
				throw std::invalid_argument("Service pointer cannot be null.");

			if (ContainsService<T>())
				throw std::invalid_argument("A service of this type is already registered.");

			items.push_back(std::make_unique<GameServiceItem<T>>(item));
		}

		// 4. Registra um ponteiro cru derivado TDerived* como TBase (nao-proprietario)
		template <typename TBase, std::derived_from<TBase> TDerived>
		void AddService(TDerived* item) {
			if (!item)
				throw std::invalid_argument("Service pointer cannot be null.");

			if (ContainsService<TBase>())
				throw std::invalid_argument("A service of this type is already registered.");

			items.push_back(std::make_unique<GameServiceItem<TBase>>(static_cast<TBase*>(item)));
		}

		// 5. Cria uma copia interna propria a partir de uma instancia
		template <typename T>
			requires (!std::is_pointer_v<T>) && std::copy_constructible<T>
		void AddService(const T& item) {
			AddService<T>(std::make_unique<T>(item));
		}

		// 6. Cria uma copia interna propria de TDerived registrada como TBase
		template <typename TBase, std::derived_from<TBase> TDerived>
			requires (!std::is_pointer_v<TDerived>) && std::copy_constructible<TDerived>
		void AddService(const TDerived& item) {
			AddService<TBase>(std::make_unique<TDerived>(item));
		}

		// Remove o servico registrado para o tipo T
		template <typename T>
		bool RemoveService() noexcept {
			const auto count = std::erase_if(items, [](const std::unique_ptr<IGameServiceItem>& entry) noexcept {
				return dynamic_cast<const GameServiceItem<T>*>(entry.get()) != nullptr;
				});

			return count > 0;
		}

		// Retorna um ponteiro observador (nao-proprietario) para o servico, ou nullptr se nao existir
		template <typename T>
		[[nodiscard]] T* GetService() const noexcept {
			for (const auto& entry : items) {
				if (const auto* item = dynamic_cast<const GameServiceItem<T>*>(entry.get())) {
					return item->ptr;
				}
			}

			return nullptr;
		}

		// Verifica se ha um servico do tipo T registrado
		template <typename T>
		[[nodiscard]] bool ContainsService() const noexcept {
			return GetService<T>() != nullptr;
		}

		// Remove todos os servicos
		void Clear() noexcept {
			items.clear();
		}

		// Retorna a quantidade de servicos registrados
		[[nodiscard]] std::size_t Count() const noexcept {
			return items.size();
		}

	private:
		struct IGameServiceItem {
			virtual ~IGameServiceItem() = default;
		};

		template <typename T>
		struct GameServiceItem final : public IGameServiceItem {
			explicit GameServiceItem(std::unique_ptr<T> ownedPtr) noexcept
				: owned(std::move(ownedPtr)), ptr(owned.get()) {}

			explicit GameServiceItem(T* rawPtr) noexcept
				: owned(nullptr), ptr(rawPtr) {}

			~GameServiceItem() override = default;

			std::unique_ptr<T> owned;
			T* ptr{ nullptr };
		};

	private:
		std::vector<std::unique_ptr<IGameServiceItem>> items;
	};
}