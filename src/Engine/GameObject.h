#pragma once
#include "Messaging.h"
#include "IReader.h"
#include <unordered_map>
#include <iostream>

#define MAKE_COMPONENT_NAME(x) #x

namespace ElysiumEngine
{
    class IComponent;
    
#ifdef ANDROID
#pragma message("using tr1!")
    typedef std::tr1::unordered_map<std::string, IComponent *> ComponentMap;
#else
#pragma message("Using std")
    typedef std::unordered_map<std::string, IComponent *> ComponentMap;
#endif
    
	class GameObject : public IEventListener
	{
    public:
        virtual ~GameObject();//Clean up any last remaining instances of this object
        
        template<typename T>
        T *getComponent(std::string name)
        {
            auto component = components.find(name);
            if(component == components.end())
                return nullptr; 
            else
                return dynamic_cast<T *>(component->second);
        }
        
        bool hasComponent(std::string name);
        
        void initialize();
        void lateInitialize();
        
        void kill(){dead = true;}
        bool isDead(){return dead;}
        
        void addComponent(IComponent *component);
        void removeComponent(IComponent *component);
        
        void sendToChildren(IMessage *message){}
        IEventListener *getParent(){return nullptr;}
        
    private:
        GameObject();
        ComponentMap components;
        bool dead;
        friend class GameObjectFactory;
	};
    
    class IComponent : public IEventListener
	{
    public:
        IComponent(std::string name);
        virtual ~IComponent(){}
        
        virtual void serialize(FileIO::IReader &reader) = 0;
        virtual void deserialize() = 0;
        
        
        virtual void initialize() = 0;
        virtual void lateInitialize() = 0;
        
        unsigned int getID(){return id;}
        std::string getName(){return name;}
        
        
        bool hasSibling(std::string name);
        
        template<typename T>
        T *getSibling(std::string name)
        {
            return owner->getComponent<T>(name);
        }
        
        void sendToChildren(IMessage *message){}
        IEventListener *getParent(){return owner;}
        
        GameObject *getOwner(){return owner;}
        
    private:
        GameObject *owner;
        unsigned int id;
        std::string name;
        friend class GameObject;
	};
}