#include "StringUtilities.h"

#define COMPONENT(x) {StringUtilities::LowerString(#x),new ElysiumEngine::TComponentAllocator<ElysiumEngine::x>()},
std::map<std::string, ElysiumEngine::IComponentAllocator *> components = {
#include "Components.h" 
};
#undef COMPONENT

inline void registerComponents()
{
	for (auto pair : components)
	{
		ElysiumEngine::GameObjectFactory::g_GameObjectFactory->registerComponentAllocator(pair.first, pair.second);
	}
}
