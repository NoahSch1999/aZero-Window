#pragma once
#include "WindowBase.hpp"

#ifdef _WIN32
#include <Windows.h>

namespace aZero::Window {
    // Platform-specific definitions
    class Window_Win32 : public WindowBase<HWND> {
    public:
        Window_Win32() = default;
        explicit Window_Win32(const Desc& desc)
            :WindowBase<HWND>(desc) {}

        virtual ~Window_Win32() override = default;

        HWND GetNativeHandle() const final override {
            return (HWND)SDL_GetPointerProperty(
                SDL_GetWindowProperties(m_Window),
                SDL_PROP_WINDOW_WIN32_HWND_POINTER,
                NULL
            );
        }

        virtual void PollEventImpl(const SDL_Event& event) = 0;
    };
}
#endif