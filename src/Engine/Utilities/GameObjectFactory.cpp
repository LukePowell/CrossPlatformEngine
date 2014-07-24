#include "GameObjectFactory.h"
#include "XMLReader.h"
#include <assert.h>

ElysiumEngine::GameObjectFactory *ElysiumEngine::GameObjectFactory::g_GameObjectFactory = nullptr;

ElysiumEngine::GameObject *ElysiumEngine::GameObjectFactory::createGameObjectFromFile(const char *filename)
{
    FileIO::XMLReader reader;
    
    if(!reader.Open(filename))
    {
        //assert(0 && "Could not open a gameobject file for reading")
        return nullptr;
    }
    
    //Read top level attributes for the game object that we
    //are going to create
    GameObject *gameObject = new GameObject();
    
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        std::string tag = reader.GetTag();
        
        IComponent *component = allocators[tag]->allocate();
        gameObject->addComponent(component);
        component->serialize(reader);
    }
    reader.GetParent();
    
    gameObject->initialize();
    gameObject->lateInitialize();
    
    objects.push_back(gameObject);
    
    return gameObject;
}

ElysiumEngine::GameObject *ElysiumEngine::GameObjectFactory::createEmptyGameObject()
{
    GameObject *gameObject = new GameObject();
    objects.push_back(gameObject);
    return gameObject;
}

ElysiumEngine::GameObjectFactory::~GameObjectFactory()
{
    for(auto allocator : allocators)
    {
        delete allocator.second;
    }
    
    allocators.clear();
    
    g_GameObjectFactory = nullptr;
}

void ElysiumEngine::GameObjectFactory::update(float dt)
{
    //Remove any dead objects from the factory
    auto it = objects.begin();
    while(it != objects.end())
    {
        if((*it)->dead)
        {
            delete *it;
            it = objects.erase(it);
            continue;
        }
        
        if(it != objects.end())
            ++it;
    }
}

int ElysiumEngine::GameObjectFactory::getNumOfObjects()
{
    return objects.size();
}

void ElysiumEngine::GameObjectFactory::destroyAll()
{
    for(auto &obj : objects)
    {
        obj->dead = true;
    }
}

ElysiumEngine::GameObjectFactory::GameObjectFactory() : ISystem("Game Object Factory")
{
    assert(g_GameObjectFactory == nullptr && "Can only have a single game object factory");
    g_GameObjectFactory = this;
}

void ElysiumEngine::GameObjectFactory::registerComponentAllocator(std::string name, ElysiumEngine::IComponentAllocator *allocator)
{
    allocators[name] = allocator;
}
