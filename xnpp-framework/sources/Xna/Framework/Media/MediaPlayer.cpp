#include "Xna/Framework/Media/MediaPlayer.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	void MediaPlayer::Play(Song const& song) {
		queue.Play(song);
		state = MediaState::Playing;
	}

	void MediaPlayer::Play(SongCollection const& songs) {
		queue.Play(songs);
		state = MediaState::Playing;
	}

	void MediaPlayer::Play(SongCollection const& songs, size_t index) {
		queue.Play(songs, index);
		state = MediaState::Playing;
	}

	void MediaPlayer::Pause() {
		if (state != MediaState::Playing)
			return;

		Platform::IMediaPlayer::GetInstance().Pause();
		state = MediaState::Paused;
	}

	void MediaPlayer::Resume() {
		if (state == MediaState::Playing)
			return;

		Platform::IMediaPlayer::GetInstance().Resume();
		state = MediaState::Playing;
	}

	void MediaPlayer::Stop() {
		if (state == MediaState::Stopped)
			return;

		Platform::IMediaPlayer::GetInstance().Stop();
		state = MediaState::Stopped;
	}

	void MediaPlayer::MoveNext() {
		queue.MoveNext();
		state = MediaState::Playing;
	}

	void MediaPlayer::MovePrevious() {
		queue.MovePrevious();
		state = MediaState::Playing;
	}

	void MediaPlayer::Volume(float value) {		
		Platform::IMediaPlayer::GetInstance().SetVolume(value);
	}

	void MediaPlayer::IsMuted(bool value) {
		if (isMuted == value)
			return;
		
		Platform::IMediaPlayer::GetInstance().SetMuted(value);
	}

	void MediaPlayer::IsRepeating(bool value) {
		if (isRepeating == value)
			return;

		Platform::IMediaPlayer::GetInstance().SetIsRepeating(value);
	}

	void MediaPlayer::OnActiveSongChanged(CSharp::EventArgs const& args) {
		activeSongChanged.Invoke(nullptr, args);
	}

	void MediaPlayer::OnMediaStateChanged(CSharp::EventArgs const& args) {
		mediaStateChanged.Invoke(nullptr, args);
	}
}