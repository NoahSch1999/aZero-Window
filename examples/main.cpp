#include "aZeroWindow.hpp"

#ifdef COMPILE_EXAMPLE
class RenderWindow : public aZero::Window::Window_Win32 {
public:
	RenderWindow() = default;
	explicit RenderWindow(const aZero::Window::WindowDesc& desc)
		:Window_Win32(desc)
	{
	}
private:
	virtual void PollEventImpl(const SDL_Event& event) override {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			this->Close();
		}
	}
};

// API EXAMPLE
int main(int argc, char* argv[]) {
	aZero::Window::Init();

	RenderWindow window(aZero::Window::WindowDesc("MyWindow", { 0,0,800,600 }, { 255,255,0,0 }, SDL_WINDOW_RESIZABLE, {}));

	const HWND handle = window.GetNativeHandle(); // Use for swapchain creation

	while (window.IsOpen()) {
		window.PollEvents();
	}

	aZero::Window::Shutdown();

	return 0;
}
#endif