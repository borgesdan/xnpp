#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Audio/SoundEffect.hpp""
#include "Xna/Framework/Audio/SoundEffectInstance.hpp"
#include "Xna/Framework/Audio/DynamicSoundEffectInstance.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Framework/Media/Song.hpp"

namespace Xna {
	void Platform::SoundEffect_Create(SoundEffect const& se, std::vector<uint8_t> const& format, std::vector<uint8_t> const& data,
		size_t offset, size_t count, size_t loopStart, size_t loopLength) {
		auto wfx = reinterpret_cast<const WAVEFORMATEX*>(format.data());
		
		//O ponteiro WAVEFORMATEX* precisa persistir em memória para que não ocorra corrupção de dados
		se.impl->platformImpl.currentWaveFormatex = *wfx;

		auto pcmBuffer = std::make_unique<uint8_t[]>(count);
		std::memcpy(pcmBuffer.get(), data.data() + offset, count);
		auto audioStart = pcmBuffer.get();

		auto& audioEngine = WindowsPlatform::GetAudioEngine();

		auto ptr = std::make_unique<DirectX::SoundEffect>(
			//_In_ AudioEngine* engine,
			&audioEngine,
			//_Inout_ std::unique_ptr<uint8_t[]>& wavData,
			pcmBuffer,
			//_In_ const WAVEFORMATEX* wfx,
			&se.impl->platformImpl.currentWaveFormatex,
			// _In_reads_bytes_(audioBytes) const uint8_t* startAudio
			audioStart,
			//size_t audioBytes
			count,
			//uint32_t loopStart
			static_cast<uint32_t>(loopStart),
			//uint32_t loopLength
			static_cast<uint32_t>(loopLength));

		se.impl->platformImpl.soundEffect = std::move(ptr);
	}

	void Platform::SoundEffect_SetMasterSoundProperties(std::optional<float> volume, std::optional<float> speedOfSound, std::optional<float> dopplerScale, std::optional<float> distanceScale) {

		if (volume)
			WindowsPlatform::GetAudioEngine().SetMasterVolume(volume.value());

		if (speedOfSound)
		{
			auto voice = WindowsPlatform::GetAudioEngine().GetMasterVoice();
			//TODO
		}

		if (dopplerScale) {
			//TODO
		}

		if (distanceScale) {
			//TODO
		}
	}

	void Platform::SoundEffect_SetState(SoundEffectInstance const& se, SoundEffect_State state, bool immediateIfStop) {

		switch (state) {
		case SoundEffect_State::Play:
			se.impl->platformImpl.soundEffectInstance->Play(se.impl->looped);
			break;
		case SoundEffect_State::Stop:
			se.impl->platformImpl.soundEffectInstance->Stop(immediateIfStop);
			break;
		case SoundEffect_State::Pause:
			se.impl->platformImpl.soundEffectInstance->Pause();
			break;
		case SoundEffect_State::Resume:
			se.impl->platformImpl.soundEffectInstance->Resume();
			break;
		}		
	}

	void Platform::SoundEffect_SetAttributes(SoundEffectInstance const& se, std::optional<float> volume, std::optional<float> pan, std::optional<float> pitch) {
		if (volume) {
			se.impl->platformImpl.soundEffectInstance->SetVolume(*volume);
		}

		if (pan) {
			se.impl->platformImpl.soundEffectInstance->SetPan(*pan);
		}

		if (pitch) {
			se.impl->platformImpl.soundEffectInstance->SetPitch(*pitch);
		}		
	}

	void Platform::SoundEffect_Apply3D(SoundEffectInstance const& se, std::vector<AudioListener> const& listeners, AudioEmitter emitter) {
		X3DAUDIO_LISTENER _listener{};
		X3DAUDIO_EMITTER _emitter{};

		//TODO: somente o primeiro
		const auto& listenerData = listeners[0].GetData();

		X3DAUDIO_VECTOR vec;
		vec.x = listenerData._Position.X;
		vec.y = listenerData._Position.Y;
		vec.z = listenerData._Position.Z;
		_listener.Position = vec;

		vec.x = listenerData._Forward.X;
		vec.y = listenerData._Forward.Y;
		vec.z = listenerData._Forward.Z;
		_listener.OrientFront = vec;

		vec.x = listenerData._Up.X;
		vec.y = listenerData._Up.Y;
		vec.z = listenerData._Up.Z;
		_listener.OrientTop = vec;

		vec.x = listenerData._Velocity.X;
		vec.y = listenerData._Velocity.Y;
		vec.z = listenerData._Velocity.Z;
		_listener.Velocity = vec;

		auto& emitterData = emitter.GetData();
		_emitter.ChannelCount = emitterData.ChannelCount;
		_emitter.ChannelRadius = emitterData.ChannelRadius;
		_emitter.CurveDistanceScaler = emitterData.CurveDistanceScaler;
		_emitter.DopplerScaler = emitterData._DopplerScale;
		_emitter.InnerRadius = emitterData._innerRadius;
		_emitter.InnerRadiusAngle = emitterData._innerRadiusAngle;
		
		
		vec.x = emitterData._Forward.X;
		vec.y = emitterData._Forward.Y;
		vec.z = emitterData._Forward.Z;
		_emitter.OrientFront = vec;
				
		vec.x = emitterData._Up.X;
		vec.y = emitterData._Up.Y;
		vec.z = emitterData._Up.Z;
		_emitter.OrientTop = vec;

		vec.x = emitterData._Position.X;
		vec.y = emitterData._Position.Y;
		vec.z = emitterData._Position.Z;

		_emitter.Position = vec;

		vec.x = emitterData._Velocity.X;
		vec.y = emitterData._Velocity.Y;
		vec.z = emitterData._Velocity.Z;

		_emitter.Velocity = vec;

		se.impl->platformImpl.soundEffectInstance->Apply3D(_listener, _emitter);
	}

	void Platform::SoundEffect_CreateInstance(SoundEffectInstance const& se) {	
		auto wfx = *se.SoundEffect()->impl->platformImpl.soundEffect->GetFormat();

		assert(wfx.wFormatTag == WAVE_FORMAT_PCM ||
			wfx.wFormatTag == WAVE_FORMAT_ADPCM ||
			wfx.wFormatTag == WAVE_FORMAT_WMAUDIO2);

		assert(wfx.nChannels > 0);
		assert(wfx.nSamplesPerSec > 0);
		assert(wfx.wBitsPerSample == 8 || wfx.wBitsPerSample == 16);

		auto instance = se.SoundEffect()->impl->platformImpl.soundEffect->CreateInstance();
		se.impl->platformImpl.soundEffectInstance = std::move(instance);
	}

	void Platform::SoundEffect_DeleteInstance(SoundEffectInstance const& se) {
		se.impl->platformImpl.soundEffectInstance = nullptr;
	}

	SoundState Platform::SoundEffect_GetState(SoundEffectInstance const& se) {
		auto state = se.impl->platformImpl.soundEffectInstance->GetState();
		
		if(!state)
			return SoundState::Stopped;

		switch (state)
		{
		case DirectX::STOPPED:
			return SoundState::Stopped;			
		case DirectX::PLAYING:
			return SoundState::Playing;			
		case DirectX::PAUSED:
			return SoundState::Paused;
			break;
		default:
			return SoundState::Stopped;
		}
	}

	void Platform::SoundEffect_CreateInstance(DynamicSoundEffectInstance const& se) {
		//DirectX::AudioEngine&
		auto& audioEngine = WindowsPlatform::GetAudioEngine();
		//std::optional<AudioFormat>
		auto& format = se.impl->format;
		auto ptr = new DirectX::DynamicSoundEffectInstance(
			// _In_ AudioEngine* engine,
			&audioEngine,
			//_In_ std::function<void __cdecl(DynamicSoundEffectInstance*)> bufferNeeded,
			[](DirectX::DynamicSoundEffectInstance*) {}, 
			//int sampleRate,
			format->SampleRate, 
			//int channels,
			format->Channels, 
			//int sampleBits = 16,
			format->BitsPerSample,
			//SOUND_EFFECT_INSTANCE_FLAGS flags = SoundEffectInstance_Default
			DirectX::SoundEffectInstance_Default);	
		
		se.impl->platformImpl.dynamicSoundEffectInstance.reset(ptr);
	}

	void Platform::SoundEffect_SubmitBuffer(DynamicSoundEffectInstance const& se, std::vector<uint8_t> const& buffer, size_t offset, size_t count) {
		se.impl->platformImpl.dynamicSoundEffectInstance->SubmitBuffer(buffer.data(), static_cast<uint32_t>(offset), count);
	}

	void Platform::SoundEffect_SetState(DynamicSoundEffectInstance const& se, SoundEffect_State state, bool immediateIfStop) {
		switch (state) {
		case SoundEffect_State::Play:
			se.impl->platformImpl.dynamicSoundEffectInstance->Play();
			break;
		case SoundEffect_State::Stop:
			se.impl->platformImpl.dynamicSoundEffectInstance->Stop(immediateIfStop);
			break;
		case SoundEffect_State::Pause:
			se.impl->platformImpl.dynamicSoundEffectInstance->Pause();
			break;
		case SoundEffect_State::Resume:
			se.impl->platformImpl.dynamicSoundEffectInstance->Resume();
			break;
		}
	}

	int32_t Platform::SoundEffect_GetPendingBufferCount(DynamicSoundEffectInstance const& se) {
		return se.impl->platformImpl.dynamicSoundEffectInstance->GetPendingBufferCount();
	}

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