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

void ComponentManager::reset()
{
	componentPools.clear();
}