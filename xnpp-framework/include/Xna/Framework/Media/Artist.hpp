#ifndef XNA_FRAMEWORK_MEDIA_ARTIST_HPP
#define XNA_FRAMEWORK_MEDIA_ARTIST_HPP

#include <string>

namespace Xna {
	//Provides access to artist information in the media library.
	struct Artist final {
	public:
		//Gets the name of the Artist.
		constexpr std::string Name() const {
			return name;
		}

		//Gets the SongCollection for the Artist.
		//TODO: [!] Not implemented
		//SongCollection Songs() const { return SongCollection(); }

		//Gets the AlbumCollection for the Artist.
		//TODO: [!] Not implemented
		//AlbumCollection Albums() const;

		constexpr bool operator==(const Artist& other) const
		{
			return name == other.name;
		}

	private:
		Artist() {}
		static inline Artist Empty() { return Artist(); }

		std::string name;
		//SongCollection songs;
		//AlbumCollection albums;

		friend class Song;
		friend class Album;
		friend struct Platform;		
	};
}

#endif