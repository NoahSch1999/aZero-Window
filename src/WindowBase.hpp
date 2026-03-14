#pragma once
#include <type_traits>
#include <string>
#include <stdexcept>
#include <array>
#include <vector>

#include "SDL3/SDL.h"

namespace aZero::Window {
	struct WindowDesc {
		std::string_view title;

		struct Rect {
			int32_t x = 0;
			int32_t y = 0;
			int32_t w = 0;
			int32_t h = 0;
		};
		Rect rect;
		std::array<uint8_t, 4> clearColor;
		SDL_WindowFlags windowFlags;

		WindowDesc() = default;
		WindowDesc(std::string_view title, const Rect& rect, const std::array<uint8_t, 4>& clearColor, SDL_WindowFlags windowFlags)
			:title(title), rect(rect), clearColor(clearColor), windowFlags(windowFlags) { }

	};

	template<typename WindowHandle>
	class WindowBase {
	public:
		using Desc = WindowDesc;

		virtual WindowHandle GetNativeHandle() const = 0;

		WindowBase() = default;

		explicit WindowBase(const Desc& desc) { this->Init(desc); }

		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;

		WindowBase(WindowBase&& other) noexcept { this->Move(std::move(other)); }

		WindowBase& operator=(WindowBase&& other) noexcept {
			this->Move(std::move(other));
			return *this;
		}

		virtual ~WindowBase() { this->Destroy(); }

		SDL_Window* GetSDLWindow() const { return m_Window; }
		SDL_Surface* GetSDLSurface() const { return m_ScreenSurface; }

		void Init(const Desc& desc) {
			if (m_Window)
				this->Destroy();

			m_Window = SDL_CreateWindow(desc.title.data(), desc.rect.w, desc.rect.h, desc.windowFlags);

			if (m_Window == NULL)
				throw std::runtime_error("Failure on SDL_CreateWindow()");

			m_ScreenSurface = SDL_GetWindowSurface(m_Window);

			SDL_Rect rect;
			rect.x = desc.rect.x;
			rect.y = desc.rect.y;
			rect.h = desc.rect.h;
			rect.w = desc.rect.w;
			SDL_FillSurfaceRect(m_ScreenSurface, &rect, SDL_MapRGBA(SDL_GetPixelFormatDetails(m_ScreenSurface->format), NULL, desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]));

			if (!SDL_UpdateWindowSurface(m_Window))
				throw std::runtime_error("Failure on SDL_UpdateWindowSurface()");
		}

		void PollEvents() {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT)
					m_Quit = true;

				this->PollEventImpl(event);
			}
		}

		bool IsOpen() const { return !m_Quit; }

		void Close() { m_Quit = true; }

		// https://wiki.libsdl.org/SDL3/SDL_SetWindowTitle
		bool SetTitle(std::string_view title) { return SDL_SetWindowTitle(m_Window, title.data()); }

		// https://wiki.libsdl.org/SDL2/SDL_GetWindowTitle
		std::string GetTitle() const { return SDL_GetWindowTitle(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_SetWindowSize
		// https://wiki.libsdl.org/SDL3/SDL_SetWindowPosition
		bool SetSize(const Desc::Rect& rect) {
			const bool setSizeSucceeded = SDL_SetWindowSize(m_Window, rect.w, rect.h);
			const bool setPosSucceeded = SDL_SetWindowPosition(m_Window, rect.x, rect.y);
			m_ScreenSurface = SDL_GetWindowSurface(m_Window);
			return setSizeSucceeded && setPosSucceeded;
		}

		// https://wiki.libsdl.org/SDL3/SDL_GetWindowSize
		// https://wiki.libsdl.org/SDL3/SDL_GetWindowPosition
		Desc::Rect GetWindowDimensions() const {
			Desc::Rect rect;
			SDL_GetWindowSize(m_Window, &rect.w, &rect.h);
			SDL_GetWindowPosition(m_Window, &rect.x, &rect.y);
			return rect;
		}

		// https://wiki.libsdl.org/SDL3/SDL_GetWindowSizeInPixels
		std::tuple<int32_t, int32_t> GetClientDimensions() const {
			int32_t w, h;
			SDL_GetWindowSizeInPixels(m_Window, &w, &h);
			return std::make_tuple(w, h);
		}

		// https://wiki.libsdl.org/SDL3/SDL_SetWindowFullscreenMode
		bool SetDisplayMode(const SDL_DisplayMode* mode) { return SDL_SetWindowFullscreenMode(m_Window, mode); }

		// https://wiki.libsdl.org/SDL3/SDL_GetFullscreenDisplayModes
		std::vector<SDL_DisplayMode> GetDisplayMode(SDL_DisplayID displayID) const {
			int count = 0;
			SDL_DisplayMode** displayModes = SDL_GetFullscreenDisplayModes(displayID, &count);

			std::vector<SDL_DisplayMode> vec(count);
			for (int i = 0; i < count; i++) {
				vec[i] = *displayModes[i];
			}

			SDL_free(displayModes);

			return vec;
		}

		// https://wiki.libsdl.org/SDL3/SDL_MaximizeWindow
		bool Maximize() { return SDL_MaximizeWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_MinimizeWindow
		bool Minimize() { return SDL_MinimizeWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_RestoreWindow
		bool Restore() { return SDL_RestoreWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_ShowWindow
		bool Show() { return SDL_ShowWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_HideWindow
		bool Hide() { return SDL_HideWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_RaiseWindow
		bool Focus() { return SDL_RaiseWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_SetWindowFullscreen
		bool SetFullscreen(bool enabled) { return SDL_SetWindowFullscreen(m_Window, enabled); }

		// https://wiki.libsdl.org/SDL3/SDL_SetWindowOpacity
		bool SetOpacity(float opacity) { return SDL_SetWindowOpacity(m_Window, opacity); }

		// https://wiki.libsdl.org/SDL3/SDL_SyncWindow
		bool FlushPending() const { return SDL_SyncWindow(m_Window); }

		// https://wiki.libsdl.org/SDL3/SDL_GetWindowFlags
		SDL_WindowFlags GetWindowFlags() const { return SDL_GetWindowFlags(m_Window); }

	protected:
		SDL_Window* m_Window = nullptr;
		SDL_Surface* m_ScreenSurface = nullptr;

	private:
		bool m_Quit = false;

		virtual void PollEventImpl(const SDL_Event& event) = 0;

		void Destroy() {
			if (m_Window) {
				SDL_DestroyWindow(m_Window);
			}
		}

		void Move(WindowBase other) {
			std::swap(m_Window, other.m_Window);
			std::swap(m_ScreenSurface, other.m_ScreenSurface);
			std::swap(m_Quit, other.m_Quit);
		}
	};
}
