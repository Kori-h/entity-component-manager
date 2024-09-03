#ifndef __COMPONENT_MANAGER__
#define __COMPONENT_MANAGER__ // header guard

//===================//
//  HEADERS
//===================//

#include "pch.hpp"

//===================//
//  DECLARATIONS
//===================//

size_t const MAX_COMPONENT_COUNT = 1024;

class Pool
{
    public:
        Pool();
        virtual ~Pool() = default;

    protected:
        Pool(Pool const&) = delete;
        Pool(Pool&&) = delete;
        Pool& operator=(Pool const&) = delete;
        Pool& operator=(Pool&&) = delete;
};

template <typename T>
class ComponentPool : public Pool
{
    public:
        ComponentPool();
        void enableComponent(size_t index);
        T& getComponent(size_t index);
        void disableComponent(size_t index);
        bool isComponentActive(size_t index);
        std::vector<T>& getComponents();
        void reset();

    protected:
        std::vector<T> components;
        std::bitset<MAX_COMPONENT_COUNT> componentPool;
};

class ComponentManager
{
    public:
        ComponentManager();

        template<typename T>
        void newComponentPool();

        template<typename T> 
        ComponentPool<T>& getComponentPool();

        void reset();

    protected:
        std::unordered_map<std::type_index, std::unique_ptr<Pool>> componentPools;
};

//===================//
//  DEFINITIONS
//===================//

template<typename T>
ComponentPool<T>::ComponentPool()
    : components(MAX_COMPONENT_COUNT)
{
    componentPool.reset();
}

template<typename T>
void ComponentPool<T>::enableComponent(size_t index)
{
    if (index >= componentPool.size())
    {
        throw std::exception("Index out of bounds");
    }

    components[index] = T();
    componentPool[index] = true;
}

template<typename T>
T& ComponentPool<T>::getComponent(size_t index)
{
    if (index >= componentPool.size())
    {
        throw std::exception("Index out of bounds");
    }

    return components[index];
}

template<typename T>
void ComponentPool<T>::disableComponent(size_t index)
{
    if (index >= componentPool.size())
    {
        throw std::exception("Index out of bounds for component pool.");
    }

    componentPool[index] = false;
}

template<typename T>
bool ComponentPool<T>::isComponentActive(size_t index)
{
    if (index >= componentPool.size())
    {
        throw std::exception("Index out of bounds for component pool");
    }

    return componentPool[index];
}

template<typename T>
void ComponentPool<T>::reset()
{
    componentPool.reset();
}

template<typename T>
void ComponentManager::newComponentPool()
{
    std::type_index typeIndex = std::type_index(typeid(T));

    if (componentPools.find(typeIndex) != componentPools.end())
    {
        throw std::exception("Component pools already contain a pool of the referenced type");
    }

    componentPools[typeIndex] = std::make_unique<ComponentPool<T>>();
}

template<typename T>
ComponentPool<T>& ComponentManager::getComponentPool()
{
    std::type_index typeIndex = std::type_index(typeid(T));

    if (componentPools.find(typeIndex) == componentPools.end())
    {
        throw std::exception("Component pools does not contain a pool of the referenced type");
    }

    return *static_cast<ComponentPool<T>*>(componentPools[typeIndex].get());
}

#endif __COMPONENT_MANAGER__