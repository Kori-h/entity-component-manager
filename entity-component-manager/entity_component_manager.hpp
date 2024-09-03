#ifndef __ENTITY_COMPONENT_MANAGER__
#define __ENTITY_COMPONENT_MANAGER__ // header guard

//===================//
//  HEADERS
//===================//

#include "pch.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"

//===================//
//  DECLARATIONS
//===================//

using Entity = size_t;

class EntityUpdateFunctionBase
{
	friend class EntityComponentManager;

	public:
		EntityUpdateFunctionBase() = default;
		virtual ~EntityUpdateFunctionBase() = default;
		virtual void update(EntityComponentManager& ecm, Entity entity) = 0;

	protected:
		EntityUpdateFunctionBase(EntityUpdateFunctionBase const&) = delete;
		EntityUpdateFunctionBase(EntityUpdateFunctionBase&&) = delete;
		EntityUpdateFunctionBase& operator=(EntityUpdateFunctionBase const&) = delete;
		EntityUpdateFunctionBase& operator=(EntityUpdateFunctionBase&&) = delete;
};

template<typename T, typename U>
class EntityUpdateFunction : public EntityUpdateFunctionBase
{
	public:
		EntityUpdateFunction(std::function<void(T&, U&)> function);
		void update(EntityComponentManager& ecm, Entity entity) override;

	protected:
		std::function<void(T&, U&)> function;
};

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

		template<typename T>
		ComponentPool<T>& getComponentPool();

		template<typename T, typename U>
		void registerFunction(std::function<void(T&, U&)> function);

		void registerSystem(std::function<void(EntityComponentManager&)> system);

		std::vector<size_t>& getAvailableEntities();

		void update();
		void reset();

	protected:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::vector<std::unique_ptr<EntityUpdateFunctionBase>> functions;
		std::vector<std::function<void(EntityComponentManager&)>> systems;
};

//===================//
//  DEFINITIONS
//===================//

template<typename T, typename U>
EntityUpdateFunction<T, U>::EntityUpdateFunction(std::function<void(T&, U&)> function) :
	function(function)
{

}

template<typename T, typename U>
void EntityUpdateFunction<T, U>::update(EntityComponentManager& ecm, Entity entity)
{
	if (ecm.hasComponent<T>(entity) && ecm.hasComponent<U>(entity))
	{
		function(ecm.getComponent<T>(entity), ecm.getComponent<U>(entity));
	}
}

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

template<typename T>
ComponentPool<T>& EntityComponentManager::getComponentPool()
{
	return componentManager->getComponentPool<T>();
}

template<typename T, typename U>
void EntityComponentManager::registerFunction(std::function<void(T&, U&)> function)
{
	functions.push_back
	(
		std::make_unique<EntityUpdateFunction<T, U>>
		(
			function
		)
	);
}
#endif __ENTITY_COMPONENT_MANAGER__