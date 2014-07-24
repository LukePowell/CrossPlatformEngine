#pragma once
#include "ISystem.h"
#include "GameObject.h"
#include <list>
#include <unordered_map>

namespace ElysiumEngine
{
    class IComponentAllocator
    {
    public:
        virtual ~IComponentAllocator(){}
        virtual IComponent *allocate() = 0;
    };
    
    template<typename T>
    class TComponentAllocator : public IComponentAllocator
    {
    public:
        IComponent *allocate()
        {
            return new T();
        }
    };
    
#ifdef ANDROID
    typedef std::tr1::unordered_map<std::string, IComponentAllocator *> AllocatorMap;
#else
    typedef std::unordered_map<std::string, IComponentAllocator *> AllocatorMap;
#endif
    
	class GameObjectFactory : public ISystem
	{
	public:
        
        GameObjectFactory();
        
        void update(float dt);
        
		GameObject *createGameObjectFromFile(const char *filename);
		GameObject *createEmptyGameObject();
        ~GameObjectFactory();
        void registerComponentAllocator(std::string name, IComponentAllocator *allocator);
        
        void destroyAll();
        
        int getNumOfObjects();
        
        template<typename T>
        void forAllGameObjects(T func)
        {
            for(auto &obj : objects)
            {
                func(obj);
            }
        }
        
        static GameObjectFactory *g_GameObjectFactory;
	private:
		std::list<GameObject *> objects;
        AllocatorMap allocators;
	};
}