//===================//
//  HEADERS
//===================//

#include "component_manager.hpp"

//===================//
//  DEFINITIONS
//===================//

ComponentManager::ComponentManager()
{
	componentPools.clear();
}

void ComponentManager::disableComponents(size_t index)
{
	for (auto it = componentPools.begin(); it != componentPools.end(); ++it)
	{
		it->second.get()->set(index, false);
	}
}

void ComponentManager::reset()
{
	componentPools.clear();
}