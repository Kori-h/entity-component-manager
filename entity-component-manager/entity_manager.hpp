#ifndef __ENTITY_MANAGER__
#define __ENTITY_MANAGER__ // header guard

//===================//
//  HEADERS
//===================//

#include "pch.hpp"

//===================//
//  DECLARATIONS
//===================//

size_t const MAX_ENTITY_COUNT = 1024;

class EntityManager
{ 
    public:
        EntityManager();
        size_t newEntity();
        void deleteEntity(size_t index);
        std::vector<size_t>& getActiveEntities();
        void reset();

    protected:
        size_t marker;
        std::vector<size_t> activeEntities;
        std::bitset<MAX_ENTITY_COUNT> entityPool;
};

#endif __ENTITY_MANAGER__