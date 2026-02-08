#ifndef XNA_CONTENTPIPELINE_CONTENTPROCESSOR_HPP
#define XNA_CONTENTPIPELINE_CONTENTPROCESSOR_HPP

#include <Xna/CSharp/Type.hpp>
#include <Xna/CSharp/Exception.hpp>
#include <any>
#include "ContentProcessorContext.hpp"
#include <Xna/Internal/TypeRegistry.hpp>

namespace Xna {
	struct IContentProcessor {
		virtual ~IContentProcessor() = default;
		virtual std::any Process(std::any& input, ContentProcessorContext& context) = 0;
		virtual CSharp::Type InputType() const = 0;
		virtual CSharp::Type OutputType() const = 0;
	};

	template <typename TInput, typename TOutput>
	struct ContentProcessor : public IContentProcessor {
		~ContentProcessor() override = default;

		virtual TOutput ProcessT(TInput& input, ContentProcessorContext& context) = 0;
		
		std::any Process(std::any& input, ContentProcessorContext& context) override {
			if (!input.has_value())
				throw CSharp::ArgumentNullException("input");

			try {
				auto input1 = std::any_cast<TInput>(input);
				return ProcessT(input1, context);
			}
			catch (std::exception const& ex) {
				throw CSharp::InvalidOperationException(ex.what());
			}
		}

		CSharp::Type InputType() const override { return CSharp::typeof<TInput>(); }
		CSharp::Type OutputType() const override { return CSharp::typeof<TOutput>(); }
	};
}

#endif