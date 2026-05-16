#include "Xna/Platform/Platform.hpp"
#include <algorithm>
#include <SDL3/SDL.h>
#include "Internal.hpp"
#include <stdexcept>

#ifdef PLATFORM_WINDOWS
#define NOMINMAX
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>
#include <limits.h>
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif

namespace Xna {
	void Cross::System::Initialize() {
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			throw std::runtime_error(SDL_GetError());
		}

		if (!SDL_Init(SDL_INIT_GAMEPAD)) {
			throw std::runtime_error(SDL_GetError());
		}

		InitAudio();
	}

	void Cross::System::Dispose() {
		DisposeAudio();
		DisposeVideo();
	}

	size_t Platform::System::GetClockCounter() {
		return static_cast<size_t>(SDL_GetPerformanceCounter());
	}

	size_t Platform::System::GetClockFrequency() {
		return static_cast<size_t>(SDL_GetPerformanceFrequency());
	}

	bool Platform::System::MultiMonitorSupport() {
		int count = 0;
		auto displays = SDL_GetDisplays(&count);

		if (displays)
			SDL_free(displays);

		return count > 1;
	}		

	void Platform::System::GetExecutablePath(std::filesystem::path& path) {
#ifdef PLATFORM_WINDOWS
		wchar_t buffer[MAX_PATH]{};
		const auto length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length > 0)
			path = std::filesystem::path(buffer, buffer + length);
#elif PLATFORM_MACOS
		char buffer[PATH_MAX];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0) {
			path = std::filesystem::path(buffer);
		}
#elif PLATFORM_LINUX
		char buffer[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
		if (count != -1) {
			path = std::filesystem::path(buffer, buffer + count);
		}		
#endif
	}

	void Platform::System::ProcessException(std::string const& exception) {
#ifdef PLATFORM_WINDOWS
		MessageBox(nullptr, exception.c_str(), "XN++", MB_OK);		
#elif
		SDL_Log(exception.c_str());
#endif
	}

	void Platform::System::Initialize() {
		Cross::System::Initialize();
	}

	void Platform::System::Dispose() {
		Cross::System::Dispose();
	}

	void Platform::System::Update() {
	}

	void Platform::System::Suspend() {
	}

	void Platform::System::Resume() {
	}
}