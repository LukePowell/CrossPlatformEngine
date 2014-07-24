#include "SFMLWindow.h"
#include <iostream>

ElysiumEngine::SFMLWindow::SFMLWindow(std::string name, int resX, int resY, bool fullscreen /* = false*/) : IWindow(name, resX, resY, fullscreen)
{
    window = nullptr;
}

bool ElysiumEngine::SFMLWindow::create()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    window = new sf::RenderWindow(sf::VideoMode(resX,resY),title, sf::Style::Default,settings);

	return true;
}

void ElysiumEngine::SFMLWindow::update(float dt)
{
    if(!window)
        return;
    
    sf::Event event;
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            if(window->isOpen())
            {
                window->close();
                window = nullptr;
                return;
            }
        }
        else if(event.type == sf::Event::Resized)
        {
            resX = event.size.width;
            resY = event.size.height;
            
            WindowResizedMessage msg(event.size.width,event.size.height);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            int key = event.key.code;
            
            if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
            {
                key += 'a';
                if(event.key.code == sf::Keyboard::Q)
                {
                    if(event.key.system)
                    {
                        window->close();
                        window = nullptr;
                        return;
                    }
                }
            }
            if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
            {
                key -= sf::Keyboard::Num0;
                key += '0';
            }
            else if(event.key.code == sf::Keyboard::Space)
            {
                key = ' ';
            }
     
            KeyStateMessage msg(key,true);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            int key = event.key.code;
            
            if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
            {
                key += 'a';
            }
            if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
            {
                key -= sf::Keyboard::Num0;
                key += '0';
            }
            else if(event.key.code == sf::Keyboard::Space)
            {
                key = ' ';
            }
            
            KeyStateMessage msg(key,false);
            MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
        }
        else if(event.type == sf::Event::TextEntered)
        {
            if(event.text.unicode >= 32 && event.text.unicode <= 126)
            {
                TextMessage msg(event.text.unicode);
                MessagingSystem::g_MessagingSystem->broadcastMessage(&msg);
            }
        }
    }
}

bool ElysiumEngine::SFMLWindow::isOpen()
{
    if(window == nullptr)
        return false;
    else
        return this->window->isOpen();
}

void ElysiumEngine::SFMLWindow::redraw()
{
    if(window == nullptr)
        return;
    
    window->display();
}

void ElysiumEngine::SFMLWindow::onSetTitle(std::string title)
{
    window->setTitle(title);
}