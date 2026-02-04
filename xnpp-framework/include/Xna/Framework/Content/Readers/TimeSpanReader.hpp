#ifndef XNA_CONTENT_READERS_TIMESPANREADER_HPP
#define XNA_CONTENT_READERS_TIMESPANREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/CSharp/TimeSpan.hpp"

namespace Xna {
	struct TimeSpanReader : public ContentTypeReaderT<CSharp::TimeSpan> {
	protected:
		inline CSharp::TimeSpan Read(ContentReader& input, CSharp::TimeSpan& existingInstance) override {
			return CSharp::TimeSpan::FromTicks(input.ReadInt64());
		}
	};
}

#endif