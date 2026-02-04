#ifndef XNA_FRAMEWORK_CURVEKEYCOLLECTION_HPP
#define XNA_FRAMEWORK_CURVEKEYCOLLECTION_HPP

#include "Shared.hpp"
#include "CurveKey.hpp"

namespace Xna {
	//Contains the CurveKeys making up a Curve. 
	//[!] To ensure CurveKeyCollection methods are constexpr, we decided 
	// not to inherit this struct from the ICollection<CurveKey> interface.
	struct CurveKeyCollection final {
		//Initializes a new instance of CurveKey.
		constexpr CurveKeyCollection() = default;

		//Determines the index of a CurveKey in the CurveKeyCollection.
		constexpr size_t IndexOf(CurveKey const& item) {
			auto it = std::find(keys.begin(), keys.end(), item);
			auto value = std::distance(it, keys.begin());

			return static_cast<size_t>(value);
		}

		//Removes the CurveKey at the specified index.
		constexpr void RemoveAt(size_t index) {
			if (index >= keys.size())
				return; //TODO: lançar exceção

			keys.erase(keys.begin() + index);
			IsCachedAvailable = false;
		}

		//Gets or sets the element at the specified index.
		constexpr CurveKey& operator[](size_t index) {
			if (index >= keys.size())
				index = keys.size() - 1; //TODO lançar exceção?

			return keys[index];
		}

		//Adds a CurveKey to the CurveKeyCollection.
		constexpr void Add(CurveKey const& item) {
			auto it = std::lower_bound(
				keys.begin(),
				keys.end(),
				item,
				[](CurveKey const& a, CurveKey const& b) {
					return a.Position < b.Position;
				});

			while (it != keys.end() && (*it).Position == item.Position) {
				++it;
			}

			keys.insert(it, item);

			IsCachedAvailable = false;
		}

		//Removes all CurveKeys from the CurveKeyCollection.
		constexpr void Clear() {
			keys.clear();
			TimeRange = 0.0f;
			InvTimeRange = 0.0f;
			IsCachedAvailable = false;
		}

		//Determines whether the CurveKeyCollection contains a specific CurveKey.
		constexpr bool Contains(CurveKey const& item) const {
			return std::find(keys.begin(), keys.end(), item) != keys.end();
		}

		//Gets the number of elements contained in the CurveKeyCollection.
		constexpr size_t Count() const { return keys.size(); }

		//Removes the first occurrence of a specific CurveKey from the CurveKeyCollection.
		constexpr bool Remove(CurveKey const& item) {
			auto it = std::find(keys.begin(), keys.end(), item);

			if (it == keys.end())
				return false;

			keys.erase(it);

			IsCachedAvailable = false;
		}

	private:
		friend struct Curve;
		std::vector<CurveKey> keys;

		float TimeRange{ 0.0F };
		float InvTimeRange{ 0.0F };
		bool IsCachedAvailable{ true };

		constexpr void ComputeCacheValues() {
			TimeRange = 0.0F;
			InvTimeRange = 0.0f;

			if (keys.size() > 1) {
				TimeRange = keys[keys.size() - 1].Position - keys[0].Position;

				if (TimeRange > 1.4012984643248171E-45)
					InvTimeRange = 1.0f / TimeRange;
			}

			IsCachedAvailable = true;
		}
	};
}

#endif