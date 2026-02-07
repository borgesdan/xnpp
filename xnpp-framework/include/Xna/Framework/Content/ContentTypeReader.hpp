#ifndef XNA_FRAMEWORK_CONTENT_CONTENTTYPEREADER_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTTYPEREADER_HPP

#include "Xna/CSharp/Type.hpp"
#include <any>
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	class ContentTypeReaderManager;
	class ContentReader;

	//Worker for reading a specific managed type from a binary format.
	class ContentTypeReader {
	protected:
		//Creates a new instance of ContentTypeReader.
		ContentTypeReader(CSharp::Type const& type) 
			: targetType(type), TargetIsValueType(type.IsValueType()) {}

		//Retrieves and caches nested type readers. Called by the framework at creation time.
		virtual void Initialize(ContentTypeReaderManager& manager) {}
		// Reads a strongly typed object from the current stream.
		virtual std::any Read(ContentReader& input, std::any& existingInstance) = 0;		
		const bool TargetIsValueType{ true };

	public:
		//Gets the type handled by this reader component.
		CSharp::Type TargetType() const { return targetType; }
		// 	Gets a format version number for this type.
		virtual int32_t TypeVersion() const { return 0; }
		//Determines if deserialization into an existing object is possible.
		virtual bool CanDeserializeIntoExistingObject() const { return false; }

		virtual CSharp::Type GetType() const {
			return CSharp::typeof<ContentTypeReader>();
		}

	private:
		const CSharp::Type targetType;

		friend class ContentReader;
		friend class ContentTypeReaderManager;
	};

	template <typename T>
	class ContentTypeReaderT : public ContentTypeReader {
	protected:
		ContentTypeReaderT() : ContentTypeReader(CSharp::typeof<T>()){}

		virtual std::any Read(ContentReader& input, std::any& existingInstance) override {
			if (!existingInstance.has_value())
				return T();
			else if (existingInstance.type() != typeid(T)) {
				throw CSharp::InvalidOperationException("BadXnbWrongType");
			}

			auto existingInstance1 = std::any_cast<T>(existingInstance);

			return Read(input, existingInstance1);
		}

		virtual T Read(ContentReader& input, T& existingInstance) = 0;

		virtual CSharp::Type GetType() const override {
			return CSharp::typeof<ContentTypeReaderT<T>>();
		}

		friend class ContentReader;
		friend class ContentTypeReaderManager;
	};
}

#endif