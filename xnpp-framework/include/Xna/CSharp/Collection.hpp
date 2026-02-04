#ifndef XNA_CSHARP_COLLECTION_HPP
#define XNA_CSHARP_COLLECTION_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "Exception.hpp"

namespace Xna::CSharp {
	template <class T> class IEnumerable {
	public:
		virtual ~IEnumerable() = default;
		virtual typename std::vector<T>::iterator begin() = 0;
		virtual typename std::vector<T>::iterator end() = 0;
		virtual typename std::vector<T>::iterator begin() const = 0;
		virtual typename std::vector<T>::iterator end() const = 0;
	};

	template <class T> class ICollection : public virtual IEnumerable<T> {
	public:
		virtual ~ICollection() = default;
		virtual size_t Count() const = 0;
		virtual bool IsReadOnly() const = 0;
		virtual void Add(T const& item) = 0;
		virtual void Clear() = 0;
		virtual bool Contains(T const& item) const = 0;
		virtual void CopyTo(std::vector<T>& array, size_t arrayIndex) const = 0;
		virtual bool Remove(T const& item) = 0;
	};

	template <class T> class IList : public virtual ICollection<T> {
	public:
		virtual ~IList() = default;
		virtual T& operator[](size_t index) const = 0;
		virtual int32_t IndexOf(T const& item) const = 0;
		virtual void Insert(size_t index, T const& item) = 0;
		virtual void RemoveAt(size_t index) = 0;
	};

	//Provides the base class for a generic collection.
	template <class T> class Collection : public IList<T> {
	public:
		inline Collection() {
			items = std::make_shared<std::vector<T>>();
		}

		inline Collection(std::vector<T>& vec) {
			items = std::make_shared<std::vector<T>>(std::move(vec));		
		}

		virtual ~Collection() override = default;

		Collection(std::shared_ptr<std::vector<T>> const& list) : items(list) {}

		inline size_t Count() const { return items->size(); }

		T& operator[](size_t index) const override {
			if (index >= items->size())
				throw CSharp::ArgumentOutOfRangeException();

			return items->at(index);
		}

		bool IsReadOnly() const override { return false; }

		void Add(T const& item) override { InsertItem(items->size(), item); }

		void Clear() override { ClearItems(); }

		void CopyTo(std::vector<T>& array, size_t arrayIndex) const override {}

		bool Contains(T const& item) const override {
			return std::find(items->begin(), items->end(), item) != items->end();
		}

		std::vector<T>::iterator begin() override { return items->begin(); }
		std::vector<T>::iterator end() override { return items->end(); }
		std::vector<T>::iterator begin() const override { return items->begin(); }
		std::vector<T>::iterator end() const override { return items->end(); }

		int32_t IndexOf(T const& item) const override {
			const auto it = std::find(items->begin(), items->end(), item);
			
			if (it != items->end()) 
				return static_cast<int32_t>(std::distance(items->begin(), it));
			
			return static_cast<int32_t>(-1);
		}

		void Insert(size_t index, T const& item) override {
			InsertItem(index, item);
		}

		inline bool Remove(T const& item) override {
			const auto num = IndexOf(item);

			if (num < 0)
				return false;

			RemoveItem(num);
			return true;
		}

		inline void RemoveAt(size_t index) override {
			if (index >= items->size())
				throw CSharp::ArgumentOutOfRangeException();

			RemoveItem(index);
		}

		inline Collection(std::nullptr_t) { items = nullptr; }
		inline bool operator==(Collection const& other) const noexcept { return items == other.items; }
		inline bool operator==(std::nullptr_t) const noexcept { return items == nullptr; }
		inline explicit operator bool() const noexcept { return items != nullptr; }

	protected:
		inline std::shared_ptr<std::vector<T>> Items() const { return items; }
		inline virtual void ClearItems() { items->clear(); }
		virtual void InsertItem(size_t index, T const& item) { items->insert(items->begin() + index, item); }		
		inline virtual void RemoveItem(size_t index) { items->erase(items->begin() + index); }
		inline virtual void SetItem(size_t index, T const& item) { items->at(index) = item; }

		inline std::shared_ptr<std::vector<T>>& Items() { return items; }

	private:
		std::shared_ptr<std::vector<T>> items;
	};
}

#endif