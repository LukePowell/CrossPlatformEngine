//
//  Console.h
//  SFMLTest
//
//  Created by Luke Powell on 1/21/14.
//
//

#ifndef SFMLTest_Console_h
#define SFMLTest_Console_h

#pragma once

#include <DebugConsole.h>
#include <Messaging.h>

namespace sf
{
    class RectangleShape;
    class Text;
    class RenderWindow;
}

namespace ElysiumEngine
{
    class SFMLConsoleListener : public DebugLog::IConsoleReader, public IEventListener
    {
    public:
        std::string GetLine();
        virtual void sendToChildren(IMessage *message){}
		virtual IEventListener *getParent(){return nullptr;}
        virtual void handleMessage(ElysiumEngine::KeyStateMessage &msg);
        virtual void handleMessage(ElysiumEngine::TextMessage &msg);
    private:
        std::string line;
    };
    
    class SFMLConsoleWriter : public DebugLog::IConsoleWriter
    {
    public:
        SFMLConsoleWriter(sf::RenderWindow *window, SFMLConsoleListener *listener);
        void VWrite(const std::string &message, DebugLog::MessageTypes type);
        void draw();//Draw using SFML draw calls
        void Clear();
    private:
        //State needed to contain text, and background block
        sf::RectangleShape *shape;
        sf::Text *text;
        sf::RenderWindow *window;
        std::shared_ptr<DebugLog::IConsoleReader> reader;
        std::list<std::string> messages;
        std::string message;
        SFMLConsoleListener *listener;
    };
}

#endif
