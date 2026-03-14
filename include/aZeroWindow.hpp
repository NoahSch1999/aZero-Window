#pragma once
#include "Window_Win32.hpp"

namespace aZero::Window {
	inline void Init(SDL_InitFlags flags = SDL_INIT_VIDEO) {
		SDL_Init(flags);
	}

	inline void Shutdown() {
		SDL_Quit();
	}
}