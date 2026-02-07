#ifndef XNA_CONTENT_READERS_LISTREADER_HPP
#define XNA_CONTENT_READERS_LISTREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <vector>

namespace Xna {
	template <typename T>
	struct ListReader : public ContentTypeReaderT<std::vector<T>> {
	protected:
		inline void Initialize(ContentTypeReaderManager& manager) override {
			elementReader = manager.GetTypeReader(CSharp::typeof<T>());
		}

		inline std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			auto num = input.ReadInt32();
			auto& objList = existingInstance;

			while (num-- > 0)
				objList.push_back(input.ReadObject<T>(*elementReader));

			return objList;
		}

	private:
		std::shared_ptr<ContentTypeReader> elementReader;

	public:
		bool CanDeserializeIntoExistingObject() const override { return true; }
	};
}

#endif