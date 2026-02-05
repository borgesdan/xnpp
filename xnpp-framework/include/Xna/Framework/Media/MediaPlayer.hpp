#ifndef XNA_FRAMEWORK_MEDIA_MEDIAPLAYER_HPP
#define XNA_FRAMEWORK_MEDIA_MEDIAPLAYER_HPP

#include "MediaQueue.hpp"
#include "Song.hpp"
#include "VisualizationData.hpp"
#include "Xna/CSharp/Event.hpp"
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Media playback state (playing, paused, or stopped).
	enum class MediaState
	{
		//Media playback is stopped.
		Stopped,
		//Media is currently playing.
		Playing,
		//Media playback is paused.
		Paused,
	};

	//Provides methods and properties to play, pause, resume, and stop songs. 
	// MediaPlayer also exposes shuffle, repeat, volume, play position, and visualization capabilities.
	struct MediaPlayer final {
	public:
		//Plays a song or collection of songs.
		XNPP_API static void Play(Song const& song);
		//Plays a song or collection of songs.
		XNPP_API static void Play(SongCollection const& songs);
		//Plays a song or collection of songs.
		XNPP_API static void Play(SongCollection const& songs, size_t index);
		
		//Pauses the currently playing song.
		XNPP_API static void Pause();
		//Resumes a paused song.
		XNPP_API static void Resume();
		// Stops playing a song.
		XNPP_API static void Stop();
		
		//Moves to the next song in the queue of playing songs.
		XNPP_API static void MoveNext();
		//Moves to the previous song in the queue of playing songs.
		XNPP_API static void MovePrevious();
		
		//Gets or sets the repeat setting for the media player.
		static inline bool IsRepeating() { return isRepeating; }
		//Gets or sets the repeat setting for the media player.
		static void IsRepeating(bool value);
		
		//Gets or sets the shuffle setting for the media player.
		static inline bool IsShuffled() { return false; }
		//Gets or sets the shuffle setting for the media player.
		static void IsShuffled(bool value){}

		//Gets the media playback queue, MediaQueue.
		static inline MediaQueue Queue() { return queue; }
		//Gets the media playback state, MediaState.
		static inline MediaState State() { return state; }
		
		//Gets or sets the media player volume.
		static inline float Volume() { return volume; }
		//Gets or sets the media player volume.
		static void Volume(float value);
		
		//Gets or set the muted setting for the media player.
		static inline bool IsMuted() { return isMuted; }
		//Gets or set the muted setting for the media player.
		static void IsMuted(bool value);

		//Gets or sets the visualization enabled setting for the media player.
		//TODO: [!] Not implemented.
		static inline bool IsVisualizationEnabled() { return false; }
		//Gets or sets the visualization enabled setting for the media player.
		//TODO: [!] Not implemented.
		static inline void IsVisualizationEnabled(bool value) {}
		
		//Determines whether the game has control of the background music.
		//TODO: [!] Not implemented.
		static bool GameHasControl() { return true; }

		//Retrieves visualization (frequency and sample) data for the currently-playing song.
		//TODO: [!] Not implemented.
		static void GetVisualizationData(VisualizationData& visualizationData){}

		//Gets the play position within the currently playing song.		
		static CSharp::TimeSpan PlayPosition() { return queue.PlayPosition(); }

		//Raised when the active song changes due to active playback or due to explicit calls to the MoveNext or MovePrevious methods.
		CSharp::Event<CSharp::EventArgs> ActiveSongChanged() { return activeSongChanged; }
		//Raised when the media player play state changes.
		CSharp::Event<CSharp::EventArgs> MediaStateChanged() { return mediaStateChanged; }

	
		MediaPlayer() = delete;
		MediaPlayer(MediaPlayer&) = delete;
		MediaPlayer(MediaPlayer&&) = delete;

	private:
		static void OnActiveSongChanged(CSharp::EventArgs const& args);
		static void OnMediaStateChanged(CSharp::EventArgs const& args);

		static inline MediaState state = MediaState::Stopped;
		static inline MediaQueue queue = MediaQueue();
		static inline float volume = 1.0;
		static inline bool isMuted{ false };
		static inline bool isRepeating{ false };

		static inline CSharp::Event<CSharp::EventArgs> activeSongChanged = CSharp::Event<CSharp::EventArgs>();
		static inline CSharp::Event<CSharp::EventArgs> mediaStateChanged = CSharp::Event<CSharp::EventArgs>();

		friend class MediaEngineNotify;
	};
}

#endif