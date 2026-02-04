#ifndef XNA_CONTENT_READERS_DOUBLEREADER_HPP
#define XNA_CONTENT_READERS_DOUBLEREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"

namespace Xna {
	struct DoubleReader : public ContentTypeReaderT<double> {
	protected:
		inline double Read(ContentReader& input, double& existingInstance) override {
			return input.ReadDouble();
		}
	};

	struct SingleReader : public ContentTypeReaderT<float> {
	protected:
		inline float Read(ContentReader& input, float& existingInstance) override {
			return input.ReadSingle();
		}
	};
}

#endif