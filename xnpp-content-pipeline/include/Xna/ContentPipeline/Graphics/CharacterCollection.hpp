#ifndef XNA_CONTENTPIPELINE_GRAPHICS_CHARACTERCOLLECTION_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_CHARACTERCOLLECTION_HPP

#include <unordered_map>
#include <memory>
#include <cstdint>

namespace Xna {
	class CharacterCollection {
	public:
		CharacterCollection() {
			charsAdded = std::make_shared<std::unordered_map<uint32_t, bool>>();
		}

		inline void Add(uint32_t item) {
			(*charsAdded)[item] = true;
		}

		inline void Clear() {
			charsAdded->clear();
		}

		inline bool Contains(uint32_t item) const {
			return charsAdded->contains(item);
		}

		inline size_t Count() const {
			return charsAdded->size();
		}

		constexpr bool IsReadOnly() const { return false; }

		inline bool Remove(uint32_t item) {
			if (!charsAdded->contains(item))
				return false;
			
			charsAdded->erase(item);
			return true;
		}		

		std::vector<uint32_t> CharList() const {
			std::vector<uint32_t> charList;
			for (auto const& [character, exists] : *charsAdded) {
				charList.push_back(character);
			}

			return charList;
		}
		
		inline CharacterCollection(std::nullptr_t) { charsAdded = nullptr; }
		inline bool operator==(CharacterCollection const& other) const noexcept { return charsAdded == other.charsAdded; }
		inline bool operator==(std::nullptr_t) const noexcept { return charsAdded == nullptr; }
		inline explicit operator bool() const noexcept { return charsAdded != nullptr; }

		std::unordered_map<uint32_t, bool>::iterator begin() { return charsAdded->begin(); }
		std::unordered_map<uint32_t, bool>::iterator end() { return charsAdded->end(); }
		std::unordered_map<uint32_t, bool>::iterator begin() const { return charsAdded->begin(); }
		std::unordered_map<uint32_t, bool>::iterator end() const { return charsAdded->end(); }

	private:
		std::shared_ptr<std::unordered_map<uint32_t, bool>> charsAdded;
	};
}

#endif