#ifndef XNA_FRAMEWORK_MEDIA_GENRE_HPP
#define XNA_FRAMEWORK_MEDIA_GENRE_HPP

#include <string>

namespace Xna {
	//Provides access to genre information in the media library.
	struct Genre final {
	public:
		//Gets the name of the Genre.
		constexpr std::string Name() const {
			return name;
		}

		//Gets the SongCollection for the Genre.
		//TODO: [!] Not implemented
		//SongCollection Songs() const { return SongCollection(); }

		//Gets the AlbumCollection for the Genre.
		//TODO: [!] Not implemented
		//AlbumCollection Albums() const { return AlbumCollection(); }

		constexpr bool operator==(const Genre& other) const {
			return name == other.name;
		}

	private:
		Genre() {}

		static inline Genre Empty() { return Genre(); }

		std::string name;
		//SongCollection songs;
		//AlbumCollection albums;

		friend class Song;
		friend class Album;
		friend struct Platform;
	};
}

#endif