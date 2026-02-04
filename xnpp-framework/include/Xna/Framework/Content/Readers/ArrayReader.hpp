#ifndef XNA_CONTENT_READERS_ARRAYREADER_HPP
#define XNA_CONTENT_READERS_ARRAYREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <vector>

namespace Xna {
	template <typename T>
	struct ArrayReader : public ContentTypeReaderT<std::vector<T>> {
	protected:
		inline void Initialize(ContentTypeReaderManager& manager) override {
			elementReader = manager.GetTypeReader(CSharp::Type(typeid(T)));
		}

		inline std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			const auto length = input.ReadInt32();
			std::vector<T> objArray(static_cast<size_t>(length));

			for (size_t index = 0; index < length; ++index)
				objArray[index] = input.ReadObject<T>(*elementReader);

			return objArray;
		}

	private:
		std::shared_ptr<ContentTypeReader> elementReader;
	};
}

#endif