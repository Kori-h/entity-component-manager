#ifndef __ENTITY_COMPONENT_MANAGER__
#define __ENTITY_COMPONENT_MANAGER__ // header guard

//===================//
//  HEADERS
//===================//

#include "pch.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"
#include "system_manager.hpp"

//===================//
//  DECLARATIONS
//===================//

using Entity = size_t;

class EntityComponentManager
{
	public:
		EntityComponentManager();

		Entity createEntity();
		void destroyEntity(Entity entity);
		
		template<typename T>
		void registerComponent();

		template<typename T>
		T& createComponent(Entity entity);
		
		template<typename T>
		T& getComponent(Entity entity);

		template<typename T>
		bool hasComponent(Entity entity);

		template<typename T>
		void destroyComponent(Entity entity);
	
		template<typename T>
		std::vector<T>& getComponents();

		std::vector<size_t>& getAvailableEntities();

		void reset();

	protected:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<SystemManager> systemManager;
};

template<typename T>
void EntityComponentManager::registerComponent()
{
	componentManager->newComponentPool<T>();
}

template<typename T>
T& EntityComponentManager::createComponent(Entity entity)
{
	ComponentPool<T>& pool = componentManager->getComponentPool<T>();
	pool.enableComponent(entity);
	return pool.getComponent(entity); 
}

template<typename T>
T& EntityComponentManager::getComponent(Entity entity)
{
	ComponentPool<T>& pool = componentManager->getComponentPool<T>();
	return pool.getComponent(entity);
}

template<typename T>
 bool EntityComponentManager::hasComponent(Entity entity)
{
	 ComponentPool<T>& pool = componentManager->getComponentPool<T>();
	 return pool.isComponentActive(entity);
}

template<typename T>
void EntityComponentManager::destroyComponent(Entity entity)
{
	ComponentPool<T>& pool = componentManager->getComponentPool<T>();
	pool.enableComponent(entity);
}

template<typename T>
std::vector<T>& EntityComponentManager::getComponents()
{
	ComponentPool<T>& pool = componentManager->getComponentPool<T>();
	return pool.getComponents();
}

#endif __ENTITY_COMPONENT_MANAGER__