#ifndef XNA_CONTENT_READERS_NULLABLEREADER_HPP
#define XNA_CONTENT_READERS_NULLABLEREADER_HPP

#include "../ContentManager.hpp"
#include <optional>

namespace Xna {
	template <typename T>
	struct NullableReader : public ContentTypeReaderT<std::optional<T>> {
	protected:
		inline void Initialize(ContentTypeReaderManager& manager) override {
			underlyingTypeReader = manager.GetTypeReader(CSharp::Typeof<T>());
		}

		inline std::optional<T> Read(ContentReader& input, std::optional<T>& existingInstance) override {
			if (!input.ReadBoolean())
				return std::nullopt;

			return input.ReadRawObject<T>(*underlyingTypeReader);
		}

	private:
		ContentTypeReader* underlyingTypeReader;
	};
}

#endif