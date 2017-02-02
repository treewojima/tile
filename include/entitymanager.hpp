/* game
 * Copyright (C) 2014-2016 Scott Bishop <treewojima@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ENTITYMANAGER_HPP__
#define __ENTITYMANAGER_HPP__

#include "defines.hpp"

#include <boost/functional/hash.hpp>
#include <unordered_map>

#include "entity.hpp"
#include "events/subscriber.hpp"
#include "exceptions.hpp"
#include "logger.hpp"
#include "pool.hpp"
#include "stringable.hpp"
#include "systems/base.hpp"

namespace Exceptions
{
    class NoSuchEntity : public Base
    {
    public:
        NoSuchEntity(const Entity::UUID &uuid) : Base(error(uuid)) {}

    private:
        std::string error(const Entity::UUID &uuid)
        {
            std::ostringstream ss;
            ss << "no such entity with UUID " << uuid;
            return ss.str();
        }
    };

    class EntityExists : public Base
    {
    public:
        EntityExists(const Entity::UUID &uuid) : Base(error(uuid)) {}

    private:
        std::string error(const Entity::UUID &uuid)
        {
            std::ostringstream ss;
            ss << "entity already exists with UUID " << uuid;
            return ss.str();
        }
    };

    class NoSuchComponent : public Base
    {
    public:
        NoSuchComponent(const Entity::UUID &uuid,
                        const std::type_info &type) :
            Base(error(uuid, type)) {}

        NoSuchComponent(const Entity &entity,
                        const std::type_info &type) :
            Base(error(entity.getUUID(), type)) {}

        NoSuchComponent(const Entity *entity,
                        const std::type_info &type) :
            Base(error(entity->getUUID(), type)) {}

    private:
        std::string error(const Entity::UUID &uuid, const std::type_info &type)
        {
            std::ostringstream ss;
            ss << "entity " << uuid << " does not contain a component "
               << "of type " << boost::core::demangle(type.name());
            return ss.str();
        }
    };

    class ComponentExists : public Base
    {
    public:
        ComponentExists(const Components::Base &c) : Base(error(c)) {}
        ComponentExists(const Components::Base *c) : Base(error(*c)) {}

    private:
        std::string error(const Components::Base &c)
        {
            std::ostringstream ss;
            ss << "component of type "
               << boost::core::demangle(typeid(c).name())
               << " already exists in parent";
            return ss.str();
        }
    };
}

class EntityManager : public Stringable, public Events::Subscriber
{
public:
    typedef Entity::UUID UUID;

    EntityManager();
    ~EntityManager();

    UUID createEntity(const std::string &debugName = "");
    void destroyEntity(UUID uuid);

    template <class T>
    T *getComponent(UUID uuid);

    //void onEvent(const Events::EntityCreated &event);
    void onEvent(const Events::ComponentCreated &event);

    const std::string &getDebugName(UUID uuid) const;

    std::string toString() const;

private:
    // Entity memory pool
    // NOTE: Should this be a #define instead?
    static constexpr unsigned ENTITY_POOL_CAPACITY = 100;
    Pool<Entity, ENTITY_POOL_CAPACITY> _entityPool;

    // Subject to change based on performance considerations
    typedef Systems::Base::ComponentMap<Components::Base> ComponentMap;
    typedef std::unordered_map<UUID, ComponentMap, boost::hash<uuid::uuid>>
        EntityMap;
    EntityMap _map;

    // Moved from Entity class
    typedef std::unordered_map<UUID, std::string, boost::hash<uuid::uuid>>
        EntityDebugNameMap;
    EntityDebugNameMap _debugNameMap;
};

template <class T>
T *EntityManager::getComponent(UUID uuid)
{
    // First, see if we have an entity entry that matches uuid
    ComponentMap componentsMap;
    try
    {
        componentsMap = _map.at(uuid);
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchEntity(uuid);
    }

    // Try to find an entry for type T
    Components::Base *basePtr = nullptr;
    try
    {
        // should this use move semantics?
        basePtr = componentsMap.at(typeid(T));
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchComponent(uuid, typeid(T));
    }

    // Make sure the pointer isn't null
    if (!basePtr)
    {
        LOG_DEBUG << "component " << boost::core::demangle(typeid(T).name())
                  << " belonging to entity " << uuid << " is null";
        throw Exceptions::NoSuchComponent(uuid, typeid(T));
    }

    // Cast to the proper derived type
    T *ptr = dynamic_cast<T*>(basePtr);
    //ptr = std::move(std::static_pointer_cast<T>(basePtr));
    if (!ptr)
    {
        LOG_DEBUG << "could not cast component from Components::Base to "
                  << boost::core::demangle(typeid(T).name()) << " in entity "
                  << uuid;
        throw Exceptions::NoSuchComponent(uuid, typeid(T));
    }

    return ptr;
}

#endif
