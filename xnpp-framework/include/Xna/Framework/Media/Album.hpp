#ifndef XNA_FRAMEWORK_MEDIA_ALBUM_HPP
#define XNA_FRAMEWORK_MEDIA_ALBUM_HPP

#include "Artist.hpp"
#include "Genre.hpp"
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/CSharp/Collection.hpp"
#include "Xna/CSharp/IO/Stream.hpp"
#include <memory>


namespace Xna {

	//Provides access to an album in the media library.
	struct Album final {
	public:
		//Gets the name of the Album.
		constexpr std::string Name() const {
			return name;
		}

		//Gets the Artist of the Album.
		//TODO: [!] Not implemented
		//Xna::Artist Artist() const { return Xna::Artist::Empty(); }
		//Gets the Genre of the Album.
		//TODO: [!] Not implemented
		//Xna::Genre Genre() const { return Xna::Genre::Empty(); }
		//Gets a SongCollection that contains the songs on the album.
		//TODO: [!] Not implemented
		//SongCollection Songs() const { return SongCollection(); }
		
		//Gets the duration of the Album.
		//TODO: [!] Not implemented
		CSharp::TimeSpan Duration() const { return CSharp::TimeSpan::Zero(); }
		
		//Gets a value indicating whether the Album has associated album art.
		//TODO: [!] Not implemented
		bool HasArt() const { return false; }
		
		//Returns the stream that contains the album art image data.
		//TODO: [!] Not implemented
		std::unique_ptr<CSharp::Stream> GetAlbumArt() const { return nullptr; }
		
		//Returns the stream that contains the album thumbnail image data.
		//TODO: [!] Not implemented
		std::unique_ptr<CSharp::Stream> GetThumbnail() const { return nullptr; }

		constexpr bool operator==(const Album& other) const {
			return name == other.name;
		}

	private:
		Album() {}

		static inline Album Empty() { return Album(); }
		std::string name;

		friend class Song;
		friend struct Platform;		
	};

	class AlbumCollection final : public CSharp::Collection<Album> {

	};
}

#endif