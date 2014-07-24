//
//  Console.cpp
//  SFMLTest
//
//  Created by Luke Powell on 1/21/14.
//
//

#include "Console.h"
#include "SFML/Graphics.hpp"

std::string ElysiumEngine::SFMLConsoleListener::GetLine()
{
    return line;
}

 sf::Font font;
ElysiumEngine::SFMLConsoleWriter::SFMLConsoleWriter(sf::RenderWindow *window, SFMLConsoleListener *listener)
{
    //TODO: Needs to be based on the size of the screen just listen for a change message or some such thing
    shape = new sf::RectangleShape(sf::Vector2f(1920,1080));
    shape->setFillColor(sf::Color(125,125,125,200));
    
    text = new sf::Text();
    
    if(!font.loadFromFile("SourceCodePro-Light.ttf"))
    {
        std::cout << "Could not open font file!";
    }
    
    text->setFont(font);
    text->setCharacterSize(24);
    
    this->window = window;
    this->listener = listener;
}

void ElysiumEngine::SFMLConsoleListener::handleMessage(ElysiumEngine::KeyStateMessage &msg)
{
    if(msg.up)
    {
        if(msg.key == sf::Keyboard::BackSpace)
        {
            if(!line.empty())
            {
                line.erase(--line.end());
            }
        }
        else if(msg.key == sf::Keyboard::Return)
        {
            this->m_Parent->ParseInput(line);
            line.clear();
        }
    }
}


void ElysiumEngine::SFMLConsoleListener::handleMessage(ElysiumEngine::TextMessage &msg)
{
        line += msg.key;
}
void ElysiumEngine::SFMLConsoleWriter::VWrite(const std::string &message, DebugLog::MessageTypes type)
{
    this->message += message;
}

void ElysiumEngine::SFMLConsoleWriter::Clear()
{
    message.clear();
}

void ElysiumEngine::SFMLConsoleWriter::draw()
{
    text->setString(message);
    window->draw(*shape);
    window->draw(*text);
    
    //text->setPosition(sf::Vector2f(0,0));
    text->setString(listener->GetLine());
    window->draw(*text);
}