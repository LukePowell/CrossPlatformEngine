#include "WindowingSystem.h"
#include <iostream>

ElysiumEngine::IWindow *ElysiumEngine::WindowingSystem::createWindow(std::string title, int resX, int resY, bool fullscreen /* = false */)
{
	IWindow *newWindow = this->allocator->allocate(title,resX, resY, fullscreen);
    
    if(!newWindow->create())
    {
        std::cerr << "Error could not create the selected window.";
        //return new EmptyWindow();
    }
    
    this->windows.push_back(newWindow);
    
    return newWindow;
}

ElysiumEngine::WindowingSystem::WindowingSystem(IWindowAllocator *allocator) : ISystem("Windowing System")
{
    this->allocator = allocator;
}

void ElysiumEngine::WindowingSystem::update(float dt)
{
    for(IWindow *window : windows)
    {
        window->update(dt);
    }
}

ElysiumEngine::IWindow::IWindow(std::string name, int resX, int resY, bool fullscreen) : title(name), resX(resX), resY(resY), fullscreen(fullscreen)
{
}

void ElysiumEngine::IWindow::setTitle(std::string title)
{
    title = title;
    onSetTitle(title);
}

ElysiumEngine::WindowingSystem::~WindowingSystem()
{
    for(IWindow *window : windows)
    {
        delete window;
    }
    delete allocator;
}