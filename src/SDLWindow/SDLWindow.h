#pragma once
#include "WindowingSystem/WindowingSystem.h"
#include "SDL/SDL.h"

namespace ElysiumEngine
{
	class SDLWindow : public IWindow
	{
	public:
		SDLWindow(std::string name, int resX, int resY, bool fullscreen = false);

		bool create();
		void update(float dt);

		void onSetTitle(std::string title);

		bool isOpen();
		void redraw();
	private:
		SDL_Window *window;
        SDL_GLContext context;
        static bool SDL_INIT;
	};
}
