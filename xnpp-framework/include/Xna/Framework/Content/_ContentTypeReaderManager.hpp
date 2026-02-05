#ifndef XNA_FRAMEWORK_CONTENT__CONTENTTYPEREADERMANAGER_HPP
#define XNA_FRAMEWORK_CONTENT__CONTENTTYPEREADERMANAGER_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Xna/CSharp/Type.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class ContentTypeReader;
	class ContentReader;

	//A manager that constructs and keeps track of type reader objects.
	class ContentTypeReaderManager {
	public:
		//Looks up a reader for the specified type.
		XNPP_API std::shared_ptr<ContentTypeReader> GetTypeReader(CSharp::Type const& targetType);

	private:
		static std::vector<std::shared_ptr<ContentTypeReader>> ReadTypeManifest(size_t typeCount, ContentReader& contentReader);

		ContentTypeReaderManager(ContentReader const& contentReader);

		static std::shared_ptr<ContentTypeReader> GetTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::vector<std::shared_ptr<ContentTypeReader>>& newTypeReaders);

		static bool InstantiateTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::shared_ptr<ContentTypeReader>& reader);

		static void AddTypeReader(std::string const& readerTypeName, ContentReader const& contentReader,
			std::shared_ptr<ContentTypeReader> const& reader);

		static void RoolbackAddReaders(std::vector<ContentTypeReader> const& newTypeReaders);

		template <typename T>
		static void RollbackAddReader(std::map<T, std::shared_ptr<ContentTypeReader>>& dictionary, ContentTypeReader const& reader);

		static std::shared_ptr<ContentTypeReader> GetTypeReader(CSharp::Type const& targetType, ContentReader const& contentReader);

		static bool ContainsTypeReader(CSharp::Type const& targetType);

	public:
		struct Implementation;
		std::shared_ptr<Implementation> internalContentTypeReaderManager;
		friend class ContentReader;
	};
}

#endif