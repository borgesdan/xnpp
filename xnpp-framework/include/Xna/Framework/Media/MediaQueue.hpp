#ifndef XNA_FRAMEWORK_MEDIA_MEDIAQUEUE_HPP
#define XNA_FRAMEWORK_MEDIA_MEDIAQUEUE_HPP

#include "Song.hpp"
#include <optional>
#include <memory>
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {	
	//Provides methods and properties to access and control the queue of playing songs.
	class MediaQueue final {
	public:
		//Gets or sets the index of the current (active) song in the queue of playing songs.
		inline size_t Count() { return impl->songCollection ? impl->songCollection->Count() : 0; }		
		//Gets or sets the index of the current (active) song in the queue of playing songs.
		inline int32_t ActiveSongIndex() { return impl->activeSongIndex; }		
		//Gets the current Song in the queue of playing songs.
		inline std::optional<Song> ActiveSong() { return impl->activeSong; }
		XNPP_API Song operator[](size_t index);

	private:
		inline MediaQueue() { impl = std::make_shared<Implementation>(); }

		XNPP_API void Play(Song const& song);
		XNPP_API void Play(SongCollection const& songs);
		XNPP_API void Play(SongCollection const& songs, size_t index);
		XNPP_API void MoveNext();
		XNPP_API void MovePrevious();
		XNPP_API CSharp::TimeSpan PlayPosition() const;

		struct Implementation {
			std::optional<SongCollection> songCollection;
			std::optional<Song> activeSong;
			int32_t activeSongIndex{ -1 };
		};

		std::shared_ptr<Implementation> impl = nullptr;
		friend struct MediaPlayer;
	};
}

#endif