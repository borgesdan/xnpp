#include "Xna/Framework/Media/MediaQueue.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	void MediaQueue::Play(Song const& song) {
		if (song.impl->fileName.empty())
			throw CSharp::ArgumentNullException("song filename is empty()");
		impl->activeSong = song;

		Platform::MediaPlayer_Stop();
		Platform::MediaPlayer_Play(*impl->activeSong);
	}

	void MediaQueue::Play(SongCollection const& songCollection) {
		if (songCollection.Count() == 0)
			throw CSharp::ArgumentException("songCollection is empty.");

		impl->songCollection = songCollection;
		impl->activeSongIndex = 0;
		impl->activeSong = (*impl->songCollection)[impl->activeSongIndex];

		Play(*impl->activeSong);
	}

	void MediaQueue::Play(SongCollection const& songCollection, size_t index) {
		if (songCollection.Count() == 0)
			throw CSharp::ArgumentException("songCollection is empty.");

		if (index >= songCollection.Count())
			throw CSharp::ArgumentOutOfRangeException("index");

		impl->songCollection = songCollection;
		impl->activeSongIndex = index;
		impl->activeSong = (*impl->songCollection)[impl->activeSongIndex];

		Play(*impl->activeSong);
	}	

	void MediaQueue::MoveNext() {
		if (impl->songCollection && impl->songCollection->Count() == 0)
			return;

		if (impl->activeSongIndex < impl->songCollection->Count() - 1)
			impl->activeSongIndex += 1;
		else
			impl->activeSongIndex = 0;

		impl->activeSong = (*impl->songCollection)[impl->activeSongIndex];
		Play(*impl->activeSong);
	}

	void MediaQueue::MovePrevious() {
		if (impl->songCollection && impl->songCollection->Count() == 0)
			return;

		if (impl->activeSongIndex > 0)
			impl->activeSongIndex -= 1;
		else
			impl->activeSongIndex = impl->songCollection->Count() - 1;

		impl->activeSong = (*impl->songCollection)[impl->activeSongIndex];
		Play(*impl->activeSong);
	}

	Song MediaQueue::operator[](size_t index) {
		if (!impl->songCollection)
			throw CSharp::InvalidOperationException();

		if(index >= impl->songCollection->Count())
			throw CSharp::ArgumentOutOfRangeException("index");

		return (*impl->songCollection)[index];
	}

	CSharp::TimeSpan MediaQueue::PlayPosition() const {
		auto position = Platform::MediaPlayer_GetPlayPosition();
		return CSharp::TimeSpan(0, 0, 0, 0, position);
	}
}