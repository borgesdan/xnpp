#ifndef XNA_FRAMEWORK_MEDIA_SONG_HPP
#define XNA_FRAMEWORK_MEDIA_SONG_HPP

#include <cstdint>
#include <string>
#include <filesystem>
#include <memory>
#include "Artist.hpp"
#include "Album.hpp"
#include "Genre.hpp"
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/CSharp/Collection.hpp"

namespace Xna {
	struct Album;
	struct Genre;
	struct Artist;

	//Provides access to a song in the song library.
	class Song final {
	public:
		//Constructs a new Song object based on the specified URI.
		static Song FromUri(std::string const& name, std::filesystem::path const& uri) {
			return Song(name, uri);
		}

		//Gets the name of the Song.
		inline std::string Name() const { return impl->name; }
		//Gets the Album on which the Song appears.
		Xna::Album Album() const;
		//Gets the Genre of the Song.
		Xna::Genre Genre() const;
		//Gets the Artist of the Song.
		Xna::Artist Artist() const;
		//Gets the duration of the Song.
		CSharp::TimeSpan Duration() const;
		//Gets a value that indicates whether the song has been rated by the user.
		bool IsRated() const { return Rating() > 0; }
		//Gets the user's rating for the Song.		
		int32_t Rating() const;
		//Gets the song play count.		
		size_t PlayCount() const;
		//Gets the track number of the song on the song's Album.
		int32_t TrackNumber() const;
		//Gets a value that indicates whether the song is DRM protected content.		
		bool IsProtected() const;

		//Internal
		Song(std::string const& name, std::filesystem::path const& filename, size_t duration);
		//Internal
		Song() { impl = std::make_shared<Implementation>(); }		

		inline Song(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(Song const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	private:		
		Song(std::string const& name, std::filesystem::path const& uri) : Song(name, uri, 0) {}

		struct Implementation {
			std::string name;
			std::optional<Xna::Artist> artist;
			std::optional<Xna::Album> album;
			std::optional<Xna::Genre> genre = Genre::Empty();
			CSharp::TimeSpan duration = CSharp::TimeSpan::Zero();
			std::filesystem::path fileName;
			int32_t rating{ 0 };
			int32_t trackNumber{ 0 };
			std::optional<bool> restricted;
			int32_t playCount{ 0 };
			PlatformImpl::SongImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;

		friend struct Platform;
		friend struct MediaPlayer;
		friend class MediaQueue;
	};

	class SongCollection final : public CSharp::Collection<Song> {

	};
}

#endif