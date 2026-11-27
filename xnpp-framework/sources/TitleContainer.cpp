#include "TitleLocation.hpp"
#include <Xna/CSharp/IO.hpp>
#include <Xna/CSharp/Path.hpp>
#include <Xna/CSharp/Exception.hpp>
#include "Xna/Framework/TitleContainer.hpp"

namespace Xna {
	std::unique_ptr<Stream> TitleContainer::OpenStream(std::filesystem::path const& name) {
		if (name.empty())
			throw CSharp::ArgumentException("name");

		auto _name = GetCleanPath(name);

		if (IsCleanPathAbsolute(_name.string()))
			throw CSharp::ArgumentException("InvalidTitleContainerName");

		const auto& location = TitleLocation::Path();
		auto _location = location;
		_location.remove_filename();
		auto fullPath = CSharp::Path::Combine(_location, _name);

		std::unique_ptr<FileStream> unique;
		auto ptr = new FileStream(fullPath, FileMode::Open);
		unique.reset(ptr);

		return std::move(unique);
	}    

    int32_t TitleContainer::CollapseParentDirectory(std::string& path, int32_t position, size_t removeLength) {        
        size_t lastSlash = path.rfind('\\', position - 1);

        int startIndex = (lastSlash == std::string::npos) ? 0 : static_cast<int>(lastSlash + 1);
		        
        path.erase(startIndex, (position - startIndex) + removeLength);
		        
        return std::max(startIndex - 1, 1);
    }
}