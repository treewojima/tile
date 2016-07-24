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

    class NoSuchComponent : public Base
    {
    public:
        NoSuchComponent(const Entity &entity,
                        const std::type_info &type) :
            Base(error(entity, type)) {}

        NoSuchComponent(const Entity *entity,
                        const std::type_info &type) :
            Base(error(*entity, type)) {}


    private:
        std::string error(const Entity &entity, const std::type_info &type)
        {
            std::ostringstream ss;
            ss << "entity " << entity << " does not contain a component "
               << "of type " << boost::core::demangle(type.name());
            return ss.str();
        }
    };
}

class EntityManager : public Events::Subscriber
{
public:
    typedef Entity::UUID UUID;

    EntityManager();
    ~EntityManager();

    void initialize();
    void destroy();

    Entity *createEntity(const std::string &debugName = "");
    void destroyEntity(UUID uuid);

    Entity *getEntity(UUID uuid);

    template <class T>
    T *getComponent(UUID uuid);

    //void onEvent(const Events::EntityCreated &event);
    void onEvent(const Events::ComponentCreated &event);

	std::string toString() const;

private:
    bool _destroyed;

    // Entity memory pool
    Pool<Entity> _entityPool;

    // Subject to change based on performance considerations
    typedef Systems::Base::ComponentMap<Components::Base> ComponentMap; 
    typedef std::pair<Entity*, ComponentMap> EntityComponentsPair;
    typedef std::unordered_map<UUID,
                               EntityComponentsPair,
                               boost::hash<uuid::uuid>> EntityMap;

    EntityMap _map;
};

template <class T>
EntityManager::T *EntityManager::getComponent(UUID uuid)
{
    T *ptr = nullptr;
	EntityComponentsPair *pair;

	// First, see if we have an entity entry that matches uuid
    try
    {
		pair = &_map.at(uuid);
	}
    catch (std::out_of_range)
	{
        throw Exceptions::NoSuchEntity(uuid);
	}

	// Try to find an entry for type T
    const auto &entity = pair->first;
	auto &componentsMap = pair->second;
    Components::Base *basePtr = nullptr;
	try
	{
		// should this use move semantics?
		basePtr = componentsMap.at(typeid(T));
	}
    catch (std::out_of_range)
	{
        throw Exceptions::NoSuchComponent(entity, typeid(T));

#if 0
		std::ostringstream ss;
		ss << "no entry for component of type "
		   << boost::core::demangle(typeid(T).name())
		   << " in entity " << pair->first;
        throw Exceptions::Base(ss.str());
#endif

        std::ostringstream ss;
        ss << "no such entity for UUID " << uuid;
        throw Exceptions::Base(ss.str());
	}

	// Make sure the pointer isn't null
	if (!basePtr)
    {
        LOG_DEBUG << "component " << boost::core::demangle(typeid(T).name())
                  << " belonging to entity " << entity << " is null";
        throw Exceptions::NoSuchComponent(entity, typeid(T));

#if 0
		std::ostringstream ss;
		ss << "entry for component of type "
		   << boost::core::demangle(typeid(T).name())
		   << " in entity " << pair->first << "is null";
        throw Exceptions::Base(ss.str());
#endif
	}

	// Cast to the proper derived type
    ptr = dynamic_cast<T>(basePtr);
	//ptr = std::move(std::static_pointer_cast<T>(basePtr));
	if (!ptr)
	{
        LOG_DEBUG << "could not cast component from Components::Base to "
                  << boost::core::demangle(typeid(T).name()) << " in entity "
                  << entity;
        throw Exceptions::NoSuchComponent(entity, typeid(T));

#if 0
		std::ostringstream ss;
		ss << "could not cast component from Components::Base to "
		   << boost::core::demangle(typeid(T).name()) << " in entity "
		   << pair->first;
        throw Exceptions::Base(ss.str());
#endif
    }

    return ptr;
}

#endif
