#include "SDLWindow.h"
#include "Messaging.h"

bool ElysiumEngine::SDLWindow::SDL_INIT = false;

ElysiumEngine::SDLWindow::SDLWindow(std::string name, int resX, int resY, bool fullscreen /* = false */) : IWindow(name, resX, resY, fullscreen), window(nullptr)
{	
}

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        printf("SDL Error: %s\n", error);
        if (line != -1)
            printf(" + line: %i\n", line);
        SDL_ClearError();
    }
#endif
}

void ElysiumEngine::SDLWindow::InitSubSystems()
{

	if (!SDL_INIT)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
			SDL_INIT = false;
		}
	}

	//Should enable the correct GL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifndef WIN32
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_INIT = true;
}

bool ElysiumEngine::SDLWindow::create()
{   
	if (!SDL_INIT)
		return false;

	window = SDL_CreateWindow(title.c_str(),0,0,resX,resY,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    checkSDLError();
    
    if(!window)
        return false;
    
    context = SDL_GL_CreateContext(window);
    checkSDLError();
    
    SDL_GL_SetSwapInterval(1);
    checkSDLError();
    
	return window != nullptr;
}

void ElysiumEngine::SDLWindow::update(float dt)
{
	if(!window)
		return;

    SDL_Event event;
	//Handle events in the SDL manner
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            KeyStateMessage msg(event.key.keysym.sym,true);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
        }
        else if(event.type == SDL_KEYUP)
        {
            KeyStateMessage msg(event.key.keysym.sym,false);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
        }
    }
}

bool ElysiumEngine::SDLWindow::isOpen()
{
	if(window == nullptr)
		return false;
	else
		return true;//BAD
}

void ElysiumEngine::SDLWindow::onSetTitle(std::string title)
{
	//SET THE TITLE you silly person
}


void ElysiumEngine::SDLWindow::redraw()
{
    SDL_GL_SwapWindow(window);
}