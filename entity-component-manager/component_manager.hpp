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
        Pool() = default;
        virtual ~Pool() = default;       
        virtual void set(size_t index, bool active) = 0;

    protected:
        std::bitset<MAX_COMPONENT_COUNT> pool;

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

        void set(size_t index, bool active) override;

        void enableComponent(size_t index);
        T& getComponent(size_t index);
        void disableComponent(size_t index);
        bool isComponentActive(size_t index);
        std::vector<T>& getComponents();
        std::vector<size_t>& getActiveComponents();
        void reset();

    protected:
        std::vector<T> components;
        std::vector<size_t> activeComponents;       
};

class ComponentManager
{
    public:
        ComponentManager();

        template<typename T>
        void newComponentPool();

        template<typename T> 
        ComponentPool<T>& getComponentPool();

        void disableComponents(size_t index);
        void reset();

    protected:
        std::unordered_map<std::type_index, std::unique_ptr<Pool>> componentPools;
};

//===================//
//  DEFINITIONS
//===================//

template<typename T>
ComponentPool<T>::ComponentPool()
    : components(MAX_COMPONENT_COUNT), activeComponents()
{
    pool.reset();
}

template<typename T>
void ComponentPool<T>::set(size_t index, bool active)
{
    if (active == true)
    {
        enableComponent(index);
    }
    else
    {
        disableComponent(index);
    }
}

template<typename T>
void ComponentPool<T>::enableComponent(size_t index)
{
    if (index >= pool.size())
    {
        throw std::exception("Index out of bounds");
    }

    components[index] = T();
    pool[index] = true;
    activeComponents.push_back(index);
}

template<typename T>
T& ComponentPool<T>::getComponent(size_t index)
{
    if (index >= pool.size())
    {
        throw std::exception("Index out of bounds");
    }

    return components[index];
}

template<typename T>
void ComponentPool<T>::disableComponent(size_t index)
{
    if (index >= pool.size())
    {
        throw std::exception("Index out of bounds for component pool.");
    }

    pool[index] = false;

    std::vector<size_t>::iterator it = std::find(activeComponents.begin(), activeComponents.end(), index);
    if (it != activeComponents.end())
    {
        activeComponents.erase(it);
    }
}

template<typename T>
bool ComponentPool<T>::isComponentActive(size_t index)
{
    if (index >= pool.size())
    {
        throw std::exception("Index out of bounds for component pool");
    }

    return pool[index];
}

template<typename T>
std::vector<T>& ComponentPool<T>::getComponents()
{
    return components;
}

template<typename T>
std::vector<size_t>& ComponentPool<T>::getActiveComponents()
{
    return activeComponents;
}

template<typename T>
void ComponentPool<T>::reset()
{
    pool.reset();
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