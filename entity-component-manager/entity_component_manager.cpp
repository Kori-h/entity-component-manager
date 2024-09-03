//===================//
//  HEADERS
//===================//

#include "entity_component_manager.hpp"

//===================//
//  DEFINITIONS
//===================//

EntityComponentManager::EntityComponentManager()
{
	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>();
	systemManager = std::make_unique<SystemManager>();
}

Entity EntityComponentManager::createEntity()
{
	return entityManager->newEntity();
}

void EntityComponentManager::destroyEntity(Entity entity)
{
	entityManager->deleteEntity(entity);
}

std::vector<size_t>& EntityComponentManager::getAvailableEntities()
{
    return entityManager->getActiveEntities();
}

void EntityComponentManager::reset()
{
	entityManager->reset();
	componentManager->reset();
}