#ifndef XNA_CONTENTPIPELINE_OPAQUEDATADICTIONARY_HPP
#define XNA_CONTENTPIPELINE_OPAQUEDATADICTIONARY_HPP

#include <any>
#include <string>
#include <optional>
#include <functional>
#include "NamedValueDictionary.hpp"

namespace Xna {
	class OpaqueDataDictionary : public NamedValueDictionary<std::any> {
	public:
		inline OpaqueDataDictionary() { impl = std::make_shared<Implementation>(); }

		template <typename T>
		T GetValue(std::string const& key, T const& defaultValue) {
			std::any obj1;

			const auto result = TryGetValue(key, obj1);

			if (result && obj1.type() == typeid(T))
				return std::any_cast<T>(obj1);

			return defaultValue;
		}

		std::string GetContentAsXml() const;

		inline OpaqueDataDictionary(std::nullptr_t) : NamedValueDictionary(nullptr) { impl = nullptr; }
		inline bool operator==(OpaqueDataDictionary const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		void AddItem(std::string const& key, std::any const& value) override;
		void ClearItems() override;
		bool RemoveItem(std::string const& key) override;
		void SetItem(std::string const& key, std::any const& value) override;
		inline CSharp::Type DefaultSerializerType() const override {
			return CSharp::typeof<std::string>();
		}

	private:
		struct Implementation {
			std::optional<std::string> contentAsXml;
		};

		std::shared_ptr<Implementation> impl;
		friend struct std::hash<Xna::OpaqueDataDictionary>;
	};
}

namespace std
{
	template <>
	struct hash<Xna::OpaqueDataDictionary>
	{
		size_t operator()(const Xna::OpaqueDataDictionary& d) const noexcept
		{
			// Hash baseado na identidade da implementação
			return std::hash<const void*>{}(d ? d.impl.get() : nullptr);
		}
	};
}

#endif