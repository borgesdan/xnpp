#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Audio/SoundEffect.hpp""
#include "Xna/Framework/Audio/SoundEffectInstance.hpp"
#include "Xna/Framework/Audio/DynamicSoundEffectInstance.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Framework/Media/Song.hpp"

namespace Xna {	
	

	void Platform::Song_FromFile(Song const& song, std::filesystem::path const& filename) {
		//TODO: nada a fazer
	}

	static std::wstring MakeFileUrl(const std::wstring& path)
	{
		std::wstring url = L"file:///";
		url += path;
		std::replace(url.begin(), url.end(), L'\\', L'/');
		return url;
	}

	void Platform::MediaPlayer_Play(Song const& song) {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();		
		_bstr_t source(song.impl->fileName.wstring().c_str());

		const auto hr = mediaEngine->SetSource(source);			

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetSource() failed.");
	}	

	void Platform::MediaPlayer_Pause() {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		const auto hr = mediaEngine->Pause();

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->Pause() failed.");
	}

	void Platform::MediaPlayer_Resume() {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		const auto hr = mediaEngine->Play();

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->Play() failed.");
	}

	void Platform::MediaPlayer_Stop() {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		auto hr = mediaEngine->Pause();

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->Pause() failed.");

		hr = mediaEngine->SetCurrentTime(0);

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetCurrentTime(0) failed.");
	}	

	void Platform::MediaPlayer_SetVolume(float volume) {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		const auto hr = mediaEngine->SetVolume(volume);		

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetVolume() failed.");
	}

	void Platform::MediaPlayer_SetMuted(bool value) {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		const auto hr = mediaEngine->SetMuted(static_cast<BOOL>(value));

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetMuted() failed.");
	}

	void Platform::MediaPlayer_SetIsRepeating(bool value) {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		const auto hr = mediaEngine->SetLoop(static_cast<BOOL>(value));

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetLoop() failed.");
	}

	double Platform::MediaPlayer_GetPlayPosition() {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();		
		return mediaEngine->GetCurrentTime();
	}

	double Platform::MediaPlayer_GetDuration() {
		auto mediaEngine = WindowsPlatform::GetMediaEngine();
		return mediaEngine->GetDuration();
	}

	void Platform::MediaPlayer_SetProperty(Song const& song, MediaPlayer_MediaProperty prop) {
		Microsoft::WRL::ComPtr<IPropertyStore> pPropStore;		
		
		const auto ws = song.impl->fileName.wstring();
		PCWSTR path = ws.c_str();
		
		HRESULT hr = SHGetPropertyStoreFromParsingName(path, NULL, GPS_DEFAULT, IID_PPV_ARGS(&pPropStore));

		if FAILED(hr)
			return;

		PROPVARIANT propvar;
		PropVariantInit(&propvar);

		switch(prop) {
			case Platform::MediaPlayer_MediaProperty::Album: {
				hr = pPropStore->GetValue(PKEY_Music_AlbumTitle, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_LPWSTR) {					
					const auto str = WindowsHelpers::LPWSTRToString(propvar.pwszVal);
					song.impl->album = Album();
					song.impl->album->name = str;
				}
			}
			case Platform::MediaPlayer_MediaProperty::Genre: {
				hr = pPropStore->GetValue(PKEY_Music_Genre, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_LPWSTR) {
					const auto str = WindowsHelpers::LPWSTRToString(propvar.pwszVal);
					song.impl->genre = Genre();
					song.impl->genre->name = str;
				}
			}
			case Platform::MediaPlayer_MediaProperty::Artist: {
				hr = pPropStore->GetValue(PKEY_Music_Artist, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_LPWSTR) {
					const auto str = WindowsHelpers::LPWSTRToString(propvar.pwszVal);
					song.impl->artist = Artist();
					song.impl->artist->name = str;
				}
			}		
			case Platform::MediaPlayer_MediaProperty::Duration: {
				hr = pPropStore->GetValue(PKEY_Media_Duration, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_UI8) {
					const auto duration = propvar.uhVal.QuadPart;					
					const auto miliseconds = duration / 10000;
					song.impl->duration = CSharp::TimeSpan(0, 0, 0, 0, static_cast<int32_t>(miliseconds));
				}
			}
			case Platform::MediaPlayer_MediaProperty::TrackNumber: {
				hr = pPropStore->GetValue(PKEY_Music_TrackNumber, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_UI4) {
					const auto trackNumber = propvar.uintVal;

					song.impl->trackNumber = static_cast<int32_t>(trackNumber);
				}
			}
			case Platform::MediaPlayer_MediaProperty::IsProtected: {
				hr = pPropStore->GetValue(PKEY_DRM_IsProtected, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_BOOL) {
					song.impl->restricted = static_cast<bool>(propvar.boolVal);
				}
			}
			case Platform::MediaPlayer_MediaProperty::PlayCount: {
				hr = pPropStore->GetValue(PKEY_History_VisitCount, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_I4) {
					const auto count = propvar.uintVal;
					song.impl->playCount = static_cast<int32_t>(propvar.boolVal);
				}
			}
			case Platform::MediaPlayer_MediaProperty::Rating: {
				hr = pPropStore->GetValue(PKEY_Rating, &propvar);
				if (SUCCEEDED(hr) && propvar.vt == VT_UI4) {
					//(1 - 12 = One Star, 13-37 = Two Stars, 38-62 = Three Stars, 63-87 = Four Stars, 88-99 = Five Stars).
					const auto value = propvar.uintVal;
					auto rating = 0;

					if (value >= 1 && value <= 12)
						rating = 1;
					else if (value >= 1 && value <= 12)
						rating = 2;
					else if (value >= 38 && value <= 62)
						rating = 3;
					else if (value >= 63 && value <= 87)
						rating = 4;
					else if (value >= 88)
						rating = 4;

					song.impl->rating = rating;
				}
			}
		}

		PropVariantClear(&propvar);
	}
}

#endif