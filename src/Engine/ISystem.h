#pragma once
#include<string>
#include "Messaging.h"

namespace ElysiumEngine
{
	class ISystem : public IEventListener
	{
	public:

		ISystem(std::string name);

		std::string getName();
		int getID();

		virtual void initialize(){};
		virtual void update(float dt) = 0;
		
		virtual ~ISystem(){};
        
        IEventListener *getParent(){return nullptr;}
        void sendToChildren(IMessage *msg){}
	private:
		std::string name;
		int id;
	};
}