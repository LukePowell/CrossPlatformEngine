#include "GameObject.h"
#include "Hashing.h"

ElysiumEngine::IComponent::IComponent(std::string name)
{
    id = ElysiumEngine::hash(name.c_str());
    this->name = name;
    owner = nullptr;
}


bool ElysiumEngine::GameObject::hasComponent(std::string name)
{
	return components.find(name) != components.end();
}

void ElysiumEngine::GameObject::removeComponent(ElysiumEngine::IComponent *component)
{
	auto remove = components.find(component->getName());
	if(remove == components.end())
		return;
	delete remove->second;
    components.erase(remove);
}

ElysiumEngine::GameObject::~GameObject()
{
    //Free any memory allocated by any components
    //and remove them as registered listeners
    for(auto element : components)
    {
        //MessagingSystem::g_MessagingSystem->g_MessagingSystem->unregisterListener(this);
        delete element.second;
    }
}

void ElysiumEngine::GameObject::initialize()
{
    for(auto component : components)
    {
        component.second->initialize();
    }
}

void ElysiumEngine::GameObject::lateInitialize()
{
    for(auto component : components)
    {
        component.second->lateInitialize();
    }
}

ElysiumEngine::GameObject::GameObject() : dead(false)
{
    
}

void ElysiumEngine::GameObject::addComponent(ElysiumEngine::IComponent *component)
{
    components[component->getName()] = component;
    component->owner = this;
}

bool ElysiumEngine::IComponent::hasSibling(std::string name)
{
    return owner->hasComponent(name);
}
