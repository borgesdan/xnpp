#ifndef XNA_CONTENT_READERS_DICTIONARYREADER_HPP
#define XNA_CONTENT_READERS_DICTIONARYREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <unordered_map>

namespace Xna {
	template <typename Key, typename Value>
	struct DictionaryReader : public ContentTypeReaderT<std::unordered_map<Key, Value>> {
	protected:
		void Initialize(ContentTypeReaderManager& manager) override {
			keyReader = manager.GetTypeReader(CSharp::Type(typeid(Key)));
			valueReader = manager.GetTypeReader(CSharp::Type(typeid(Value)));
		}

		inline std::unordered_map<Key, Value> Read(ContentReader& input, std::unordered_map<Key, Value>& existingInstance) override {
			auto capacity = input.ReadInt32();
			auto& dictionary = existingInstance;
			
			while (capacity-- > 0)
			{
				const auto key = input.ReadObject<Key>(*keyReader);
				const auto obj = input.ReadObject<Value>(*valueReader);
				dictionary.emplace(key, obj );
			}
			return dictionary;
		}

	private:
		std::shared_ptr<ContentTypeReader> keyReader;
		std::shared_ptr<ContentTypeReader> valueReader;
	};
}

#endif