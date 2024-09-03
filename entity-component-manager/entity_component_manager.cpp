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
}

Entity EntityComponentManager::createEntity()
{
	return entityManager->newEntity();
}

void EntityComponentManager::destroyEntity(Entity entity)
{
	entityManager->deleteEntity(entity);
}

void EntityComponentManager::registerSystem(std::function<void(EntityComponentManager&)> system)
{
	systems.push_back(system);
}

std::vector<size_t>& EntityComponentManager::getAvailableEntities()
{
    return entityManager->getActiveEntities();
}

void EntityComponentManager::update()
{
	for (std::unique_ptr<EntityUpdateFunctionBase>& function : functions)
	{
		for (Entity entity : entityManager->getActiveEntities())
		{
			function->update(*this, entity);
		}
	}

	for (std::function<void(EntityComponentManager&)>& system : systems)
	{
		system(*this);
	}
}

void EntityComponentManager::reset()
{
	entityManager->reset();
	componentManager->reset();
}