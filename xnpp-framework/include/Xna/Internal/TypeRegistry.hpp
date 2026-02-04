#ifndef XNA_INTERNAL_TYPEREGISTRY_HPP
#define XNA_INTERNAL_TYPEREGISTRY_HPP

#include "Xna/CSharp/Type.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace Xna {
	template <typename TBASE>
	class TypeRegistry {
	public:
		template <typename T> void Add(T const& value) {
			auto pReader = std::make_shared<T>(value);
			auto pBase = dynamic_pointer_cast<TBASE>(pReader);

			auto internal = InternalType();
			internal.value = pBase;
			internal.type = typeid(T);

			values.push_back(internal);

			const auto typeName = typeid(T).name();
			auto find = CSharp::Type::NamedTypes().find(typeName);

			if (find == CSharp::Type::NamedTypes().end())
				CSharp::Type::NamedTypes().emplace(typeName, CSharp::Type(typeid(T)));

			needUpdate = true;
		}		

		template <typename T> void Add(T const& value, std::vector<std::string> const& friendlyNames) {
			auto pReader = std::make_shared<T>(value);
			auto pBase = dynamic_pointer_cast<TBASE>(pReader);

			auto internal = InternalType();
			internal.value = pBase;
			internal.type = CSharp::Type(typeid(T));			
			internal.friendlyNames = friendlyNames;

			values.push_back(internal);

			const auto typeName = typeid(T).name();
			auto find = CSharp::Type::NamedTypes().find(typeName);

			if (find == CSharp::Type::NamedTypes().end())
				CSharp::Type::NamedTypes().emplace(typeName, CSharp::Type(typeid(T)));

			for (const auto& friendlyName : friendlyNames) {
				if (CSharp::Type::NamedTypes().find(friendlyName) == CSharp::Type::NamedTypes().end())
					CSharp::Type::NamedTypes().emplace(friendlyName, CSharp::Type(typeid(T)));
			}		

			needUpdate = true;
		}

		std::shared_ptr<TBASE> CreateInstance(CSharp::Type const& type) {
			for (auto& value : values) {
				if (value.type == type)
					return value.value;
			}

			return nullptr;
		}

		std::shared_ptr<TBASE> CreateInstance(std::string const& name) {
			for (const auto& value : values) {
				if (value.type.Name() == name)
					return value.value;
			}

			for (const auto& value : values) {
				for (const auto& friendly : value.friendlyNames) {
					if (friendly == name)
						return value.value;
				}					
			}

			return nullptr;
		}

		std::optional<std::string> GetName(size_t index) {
			if (index >= values.size())
				return {};

			return values[index].type.Name();
		}

		const std::vector<std::string>& GetFriendlyNames(size_t index) {
			if (index >= values.size())
				return {};

			return values[index].friendlyNames;
		}

		const std::vector<std::shared_ptr<TBASE>>& GetRegistries() {
			UpdateObjs();
			return objs;
		}

	private:
		struct InternalType {
			CSharp::Type type;
			std::vector<std::string> friendlyNames;
			std::shared_ptr<TBASE> value;
		};

		std::vector<InternalType> values;
		std::vector<std::shared_ptr<TBASE>> objs;
		bool needUpdate = false;

		void UpdateObjs() {
			if (needUpdate) {
				objs.reserve(values.size());
				for (auto& value : values) {
					objs.push_back(value.value);
				}
			}
		}
	};
}

#endif