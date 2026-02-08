#ifndef XNA_CONTENTPIPELINE_NAMEDVALUEDICTIONARY_HPP
#define XNA_CONTENTPIPELINE_NAMEDVALUEDICTIONARY_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <Xna/CSharp/Exception.hpp>
#include <Xna/CSharp/Type.hpp>

namespace Xna {
	template <typename T>
	class NamedValueDictionary {
	public:
		inline NamedValueDictionary() {
			items = std::make_shared<std::unordered_map<std::string, T>>();
		}		
		
		inline void Add(std::string const& key, T const& value) {
			return AddItem(key, value);
		}

		inline void Clear() {
			return ClearItems();
		}

		inline bool ContainsKey(std::string const& key) const {
			return items->contains(key);
		}

		inline size_t Count() const { return items->size(); }

		inline bool Remove(std::string const& key) {
			return RemoveItem(key);
		}

		inline bool TryGetValue(std::string const& key, T& value) {
			if (!ContainsKey(key))
				return false;
			
			value = items->at(key);
			return true;
		}

		inline T& operator[](std::string const& key) {
			return items->at(key);
		}

		inline std::unordered_map<std::string, T>::iterator begin() { return items->begin(); }
		inline std::unordered_map<std::string, T>::iterator end() { return items->end(); }
		inline std::unordered_map<std::string, T>::iterator begin() const { return items->begin(); }
		inline std::unordered_map<std::string, T>::iterator end() const { return items->end(); }

		inline NamedValueDictionary(std::nullptr_t) { items = nullptr; }
		inline bool operator==(NamedValueDictionary const& other) const noexcept { return items == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return items == nullptr; }
		inline explicit operator bool() const noexcept { return items != nullptr; }
	protected:
		inline virtual void AddItem(std::string const& key, T const& value) {
			if (key.empty())
				throw CSharp::InvalidOperationException();

			items->emplace(key, value);
		}

		inline virtual void ClearItems() {
			items->clear();
		}

		virtual bool RemoveItem(std::string const& key) {
			auto it = items->find(key);

			if (it == items->end())
				return false;

			items->erase(it);
			return true;
		}

		virtual void SetItem(std::string const& key, T const& value) {
			if (key.empty())
				throw CSharp::InvalidOperationException();

			items->at(key) = value;
		}

		inline virtual CSharp::Type DefaultSerializerType() const { return CSharp::typeof<T>(); }

	private:
		std::shared_ptr<std::unordered_map<std::string, T>> items;
	};
}

#endif