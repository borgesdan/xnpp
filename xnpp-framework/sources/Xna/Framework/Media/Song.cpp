#include "Xna/Framework/Media/Song.hpp"

namespace Xna {
	Song::Song(std::string const& name, std::filesystem::path const& filename, size_t duration) {
		impl = std::make_shared<Implementation>();
		impl->name = name;
		impl->rating = 0;
		impl->trackNumber = 0;		
		impl->fileName = filename;
		impl->duration = CSharp::TimeSpan(0, 0, 0, 0, duration);		
	}

	Xna::Album Song::Album() const {
		return impl->album.value_or(Xna::Album::Empty());
	}

	Xna::Genre Song::Genre() const {
		return impl->genre.value_or(Xna::Genre::Empty());
	}

	Xna::Artist Song::Artist() const {
		return impl->artist.value_or(Xna::Artist::Empty());
	}

	CSharp::TimeSpan Song::Duration() const {
		return impl->duration;
	}

	int32_t Song::TrackNumber() const {
		return impl->trackNumber;
	}

	bool Song::IsProtected() const {
		return impl->restricted.value_or(false);
	}

	size_t Song::PlayCount() const {
		return impl->playCount;
	}

	int32_t Song::Rating() const {
		return impl->rating;
	}
}