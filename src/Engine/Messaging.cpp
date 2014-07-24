#include "Messaging.h"
#include "Hashing.h"
#include <cmath>
#include <iostream>

ElysiumEngine::MessagingSystem * ElysiumEngine::MessagingSystem::g_MessagingSystem = nullptr;

void ElysiumEngine::MessagingSystem::broadcastMessage(IMessage *message)
{
	auto receivers = messageListeners.find(message->getName());
	if(receivers != messageListeners.end())
	{
		for (auto listener : receivers->second)
		{
			message->send(*listener);
		}
	}
}

void ElysiumEngine::MessagingSystem::registerListener(std::string type, IEventListener *receiver )
{
    messageListeners[type].push_back(receiver);
}

ElysiumEngine::MessagingSystem::MessagingSystem()
{
	g_MessagingSystem = this;
}

void ElysiumEngine::IMessage::send(IEventListener &receiver )
{
	//This will send it to the intended receiver
    dispatch(receiver);
    
	//If bubbling up send to the parent
	if(bubbleUp)
	{
		IEventListener *parent = receiver.getParent();
		if(parent)
			dispatch(*parent);
	}
	else
	{
		receiver.sendToChildren(this);
	}
}

ElysiumEngine::IMessage::IMessage(std::string name) : name(name)
{
	//Get a little hashed id
	id = hash(name.c_str());
	bubbleUp = false;//By default bubble down to all the children
}
