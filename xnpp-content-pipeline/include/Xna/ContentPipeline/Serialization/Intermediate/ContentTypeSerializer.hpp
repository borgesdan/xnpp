#ifndef INTERMEDIATE_CONTENTTYPESERIALIZER_HPP
#define INTERMEDIATE_CONTENTTYPESERIALIZER_HPP

#include "IntermediateSerializer.hpp"
#include "IntermediateReader.hpp"
#include "IntermediateWriter.hpp"
#include <Xna/CSharp/Type.hpp>
#include <Xna/CSharp/Exception.hpp>
#include <Xna/Framework/Content/ContentSerializerAttribute.hpp>
#include <string>
#include <any>
#include <functional>

namespace Xna {
	//Provides methods for serializing and deserializing a specific managed type.
	struct ContentTypeSerializer {
	public:
		virtual ~ContentTypeSerializer() = default;

		inline CSharp::Type TargetType() const noexcept { return targetType; }
		inline virtual CSharp::Type BoxedTargetType() const noexcept { return targetType; }
		inline std::string XmlTypeName() const noexcept { return xmlTypeName; }
		virtual bool CanDeserializeIntoExistingObject() const noexcept { return false; }
		virtual bool ObjectIsEmpty(std::any const& value) const { return false; }

	protected:
		inline ContentTypeSerializer(CSharp::Type const& targetType) : ContentTypeSerializer(targetType, {}) {}
		inline ContentTypeSerializer(CSharp::Type const& targetType, std::string const& xmlTypeName)
			: xmlTypeName(xmlTypeName), targetType(targetType) {
		}

		virtual void Initialize(IntermediateSerializer& serializer) {};
		virtual void Serialize(IntermediateWriter output, std::any& value, ContentSerializerAttribute const& format) = 0;
		virtual std::any Deserialize(IntermediateReader input, ContentSerializerAttribute format, std::any& existingInstance) = 0;

		using ChildCallback = std::function<void(ContentTypeSerializer&, std::any const&)>;

		virtual void ScanChildren(IntermediateSerializer& serializer, ChildCallback& callback, std::any const& value) {}

	private:
		Xna::CSharp::Type targetType;
		std::string xmlTypeName;
	};

	template <typename T>
	struct ContentTypeSerializerT : public ContentTypeSerializer {
	public:
		~ContentTypeSerializerT() override = default;

		virtual bool ObjectIsEmpty(T const& value) const {
			return ObjectIsEmpty(std::any(value));
		}

		virtual bool ObjectIsEmpty(std::any const& value) const override {
			const auto value1 = CastType(value);
			return ObjectIsEmpty(value1);
		}

	protected:
		inline ContentTypeSerializerT() 
			: ContentTypeSerializer(CSharp::typeof<T>()) {}
		inline ContentTypeSerializerT(std::string const& xmlTypeName)
			: ContentTypeSerializer(CSharp::typeof<T>(), xmlTypeName) {
		}

		virtual void Serialize(IntermediateWriter output, T& value, ContentSerializerAttribute const& format) = 0;

		virtual void Serialize(IntermediateWriter output, std::any& value, ContentSerializerAttribute const& format) override {
			auto value1 = CastType(value);
			Serialize(output, value1, format);
		}

		virtual T Deserialize(IntermediateReader input, ContentSerializerAttribute format, T& existingInstance) = 0;

		virtual std::any Deserialize(IntermediateReader input, ContentSerializerAttribute format, std::any& existingInstance) override {
			auto existingInstance1 = CastType(existingInstance);
			auto existingInstance2 = Deserialize(input, format, existingInstance1);
			return  existingInstance2;
		}				

		virtual void ScanChildren(IntermediateSerializer& serializer, ChildCallback& callback, T const& value) 
		{}

		virtual void ScanChildren(IntermediateSerializer& serializer, ChildCallback& callback, std::any const& value) override
		{
			const auto value1 = CastType(value);
			ScanChildren(serializer, callback, value1);
		}

	private:		
		static T CastType(std::any const& value) {
			try {
				const auto value1 = std::any_cast<T>(value);
				return value1;
			}
			catch (std::exception& ex) {
				throw CSharp::ArgumentException("value");
			}
		}
	};
}

#endif