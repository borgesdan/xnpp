#ifndef XNA_COMPILER_CONTENTTYPEWRITER_HPP
#define XNA_COMPILER_CONTENTTYPEWRITER_HPP

#include <string>
#include <cstdint>
#include <any>
#include <Xna/Framework/Content/Pipeline/Shared.hpp>
#include <Xna/CSharp/Type.hpp>
#include <Xna/CSharp/Exception.hpp>
#include "_ContentWriter.hpp"
#include "_ContentCompiler.hpp"
#include <Xna/Internal/TypeRegistry.hpp>

namespace Xna {
	struct ContentTypeWriter {
		virtual ~ContentTypeWriter() = default;

		virtual std::string GetRuntimeReader(TargetPlatform targetPlatform) = 0;
		virtual int32_t TypeVersion() const { return 0; }
		
		//TODO: [!] avaliar
		virtual inline bool TargetIsValueType() const { return targetIsValueType; };

		inline CSharp::Type TargetType() const { return targetType; }

		inline virtual bool ShouldCompressContent(TargetPlatform targetPlatform, std::any const& value) {
			return true;
		}

	protected:
		ContentTypeWriter(CSharp::Type const& targetType) : targetType(targetType) {
			
		}

		virtual void Initialize(ContentCompiler& compiler) {

		}

		virtual void Write(ContentWriter& output, std::any const& value) = 0;		

	private:
		inline void DoInitialize(ContentCompiler& compiler) {
			//TODO: [!] implementar
			Initialize(compiler);
		}		

	private:
		CSharp::Type targetType;
		bool targetIsValueType{ true };

		friend class ContentWriter;
		friend class ContentCompiler;
	};

	template <typename T>
	struct ContentTypeWriterT : public ContentTypeWriter {
		~ContentTypeWriterT() override = default;

	protected:
		ContentTypeWriterT() : ContentTypeWriter(CSharp::Type(typeid(T))) {}
		
		virtual void WriteT(ContentWriter& output, const T* value) = 0;

		void Write(ContentWriter& output, std::any const& value){
			const auto value1 = CastType(value);
			WriteT(output, &value1);
		}

	private:
		static T CastType(std::any const& value) {
			if (!value.has_value())
				throw CSharp::ArgumentNullException("value");

			try {
				const auto value1 = std::any_cast<T>(value);
				return value1;
			}
			catch (std::exception& ex) {
				throw CSharp::ArgumentException("value");
			}
		}

		friend class ContentWriter;
	};
}

#endif