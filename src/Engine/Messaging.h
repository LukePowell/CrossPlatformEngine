#pragma once 
#include <string>
#include <map>
#include <list>

#include "TemplateConstraints.h"
#include "Maths/MathLib.h"

namespace ElysiumEngine
{
    class IEventListener;
    
    class WindowResizedMessage;
    class QuitMessage;
    
    class KeyStateMessage;
    class MouseStateMessage;
    
    class TextMessage;
    
    //Debug Messges
    class DrawDebugSphere;
    class DrawDebugLine;
    class DrawDebugBox;
    class DrawDebugPoint;
    
	class IMessage
	{
	public:
		IMessage(std::string name);
        
        virtual void send(IEventListener &listener);
        
        bool bubbleUp;
        
        const std::string &getName(){return name;}
	private:
		unsigned int id;
        std::string name;
        
        virtual void dispatch(IEventListener &receiver) = 0;
	};

    class IEventListener
    {
    public:
        virtual void handleMessage(QuitMessage &msg){}
        virtual void handleMessage(WindowResizedMessage &msg){}
        virtual void handleMessage(KeyStateMessage &msg){}
        virtual void handleMessage(MouseStateMessage &msg){}
        virtual void handleMessage(DrawDebugSphere &msg){}
        virtual void handleMessage(DrawDebugLine &msg){}
        virtual void handleMessage(DrawDebugBox &msg){}
        virtual void handleMessage(DrawDebugPoint &msg){}
        virtual void handleMessage(TextMessage &msg){}
        
        /*!********************************************
         *\brief Send the message to all children of the current
         *       receiver in the case that there are no possible children
         *       then you can set this to nothing.
         *\param  message
         *        The message to send to the children.
         **********************************************/
		virtual void sendToChildren(IMessage *message) = 0;
        
		/*!********************************************
         *\brief Returns the parent receiver to this receiver
         *       what that means is entirely up to each derivation
         *       in some cases no parent would exist thus you should
         *       return nullptr
         *\return nullptr if there is no parent else return
         *        the parent receiver else wise
         **********************************************/
		virtual IEventListener *getParent() = 0;
    private:
    };
    
    class QuitMessage : public IMessage
	{
    public:
        QuitMessage();
    private:
	};
    
    class WindowResizedMessage : public IMessage
    {
    public:
        WindowResizedMessage(int newX, int newY) : IMessage("WindowResize"), newX(newX), newY(newY) {}
        int newX, newY;
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
    };
    
    class KeyStateMessage : public IMessage
    {
    public:
        KeyStateMessage(unsigned int key, bool up) : IMessage("KeyStateMessage"), key(key), up(up){};
        
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
        
        unsigned int key;
        bool up;
    };
    
    class TextMessage : public IMessage
    {
    public:
        TextMessage(unsigned int key) : IMessage("TextMessage"), key(key) {}
        unsigned int key;
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
    };
    
    class DrawDebugSphere : public IMessage
    {
    public:
        DrawDebugSphere(Vec4 center, float radius, Vec4 color, float lifetime = 1.0f/60.0f) : IMessage("DrawDebugSphere"), center(center),lifetime(lifetime),color(color),radius(radius){}
		DrawDebugSphere(Vec4 center, Vec4 extents, Vec4 color, float lifetime = 1.0f / 60.0f) : IMessage("DrawDebugSphere"), center(center), lifetime(lifetime), color(color), radius(-1.0f), extents(extents){}
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
        Vec4 center;
        float lifetime;
        Vec4 color;
		Vec4 extents;
        float radius;//Scale x,y,z, assuming unit sphere
    };
    
    class DrawDebugLine : public IMessage
    {
    public:
        DrawDebugLine(Vec4 start, Vec4 end, Vec4 color, float lifetime = 1.0f/60.0f) : IMessage("DrawDebugLine"), start(start), end(end), color(color), lifetime(lifetime) {}
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
        Vec4 start, end, color;
        float lifetime;
    };
    
    class DrawDebugBox : public IMessage
    {
    public:
        DrawDebugBox(Vec4 com, Vec4 halfExtents, Vec4 color, float lifetime = 1.0f / 60.0f) : IMessage("DrawDebugBox"), com(com), halfExtents(halfExtents), color(color), lifetime(lifetime) {}
        
        Vec4 com;
        Vec4 halfExtents;
        Vec4 color;
        float lifetime;
        
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
    };
    
    class DrawDebugPoint : public IMessage
    {
    public:
        DrawDebugPoint(Vec4 position, Vec4 color, float lifetime = 1.0f/60.0f) : IMessage("DrawDebugPoint"), position(position), color(color), lifetime(lifetime){}
        void dispatch(IEventListener &listener){listener.handleMessage(*this);}
        
        Vec4 position;
        Vec4 color;
        float lifetime;
    };
    
 /*    template< typename MessageType, typename returnType, typename... T>
    class FunctionListener : public IEventListener
    {
    public:
        FunctionListener(returnType (*p)(T...))
        {
            functionPointer = p;
        }
        
        returnType operator()(T... t)
        {
            return functionPointer(t...);
        }

        returnType (*functionPointer)(T...);
        
        void handleMessage(MessageType &msg)
        {
            this();
        }
    };*/

    template<typename messageType>
    class BoundListener : public IEventListener
    {
    public:
        BoundListener(void (*p)(messageType &))
        {
            Derives<messageType,IMessage>();
            boundFunction = p;
        }
        
        void handleMessage(messageType &msg)
        {
            boundFunction(msg);
        }
    private:
        void (*boundFunction)(messageType &);
    };
    
    class MessagingSystem
	{
	public:
		MessagingSystem();
		void broadcastMessage(IMessage *message);//Will send message to all registered parties
		void registerListener(std::string type, IEventListener *receiver);
        
        static MessagingSystem *g_MessagingSystem;
	private:
		std::map<std::string, std::list<IEventListener *> > messageListeners;
	};
    
#define RegisterEventListener(x,y) ElysiumEngine::MessagingSystem::g_MessagingSystem->registerListener(#x,y);
}

#ifdef _DEBUG
#else
#endif