#pragma once
#include "WindowingSystem/WindowingSystem.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace ElysiumEngine
{
	class SFMLWindow : public IWindow
	{
	public:
		SFMLWindow(std::string name, int resX, int resY, bool fullscsreen = false);

		bool create();
		void update(float dt);
        
        void onSetTitle(std::string title);
        
        bool isOpen();
        void redraw();
    //private:
        sf::RenderWindow *window;
        static bool SDL_INIT;
	};
}