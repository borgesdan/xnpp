#include "Xna/Framework/Media/Song.hpp"

namespace Xna {
	Song::Song(std::string const& name, std::filesystem::path const& filename, size_t duration) {
		impl = std::make_shared<Implementation>();
		impl->name = name;
		impl->rating = 0;
		impl->trackNumber = 0;		
		impl->fileName = filename;
		impl->duration = CSharp::TimeSpan(0, 0, 0, 0, duration);

		Platform::Song_FromFile(*this, filename);
	}

	Xna::Album Song::Album() const {
		if (!impl->album) 
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::Album);

		return impl->album.value_or(Xna::Album::Empty());
	}

	Xna::Genre Song::Genre() const {
		if (!impl->genre)
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::Genre);

		return impl->genre.value_or(Xna::Genre::Empty());
	}

	Xna::Artist Song::Artist() const {
		if (!impl->artist)
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::Artist);

		return impl->artist.value_or(Xna::Artist::Empty());
	}

	CSharp::TimeSpan Song::Duration() const {
		if (impl->duration == CSharp::TimeSpan::Zero())
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::Duration);

		return impl->duration;
	}

	int32_t Song::TrackNumber() const {
		if(impl->trackNumber <= 0)
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::TrackNumber);

		return impl->trackNumber;
	}

	bool Song::IsProtected() const {
		if(!impl->restricted.has_value())
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::IsProtected);

		return impl->restricted.value_or(false);
	}

	size_t Song::PlayCount() const {
		if (impl->playCount <= 0)
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::PlayCount);

		return impl->playCount;
	}

	int32_t Song::Rating() const {
		if (impl->rating <= 0)
			Platform::MediaPlayer_SetProperty(*this, Platform::MediaPlayer_MediaProperty::Rating);

		return impl->rating;
	}
}