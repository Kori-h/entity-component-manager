//===================//
//  HEADERS
//===================//

#include "entity_manager.hpp"

//===================//
//  DEFINITIONS
//===================//

EntityManager::EntityManager()
{
    marker = 0;
    entityPool.reset();
}

size_t EntityManager::newEntity()
{
    for (size_t x = marker; x < MAX_ENTITY_COUNT; ++x)
    {
        if (entityPool[x] == false)
        {
            entityPool[x] = true;
            activeEntities.push_back(x);
            marker = x + 1;
            return x;
        }
    }

    throw std::exception("Maximum number of entities allocated from entity pool");
}

std::vector<size_t>& EntityManager::getActiveEntities()
{
    return activeEntities;
}

void EntityManager::deleteEntity(size_t index)
{
    entityPool[index] = false;
    marker = index;

    std::vector<size_t>::iterator it = std::find(activeEntities.begin(), activeEntities.end(), index);
    if (it != activeEntities.end())
    {
        activeEntities.erase(it);
    }
}

void EntityManager::reset()
{
    marker = 0;
    entityPool.reset();
}