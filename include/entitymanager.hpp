/* game
 * Copyright (C) 2014 Scott Bishop <treewojima@gmail.com>
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

#include <boost/uuid/random_generator.hpp>
#include <unordered_map>

#include "components/base.hpp"
#include "entity.hpp"
#include "events/subscriber.hpp"
#include "exception.hpp"
#include "logger.hpp"
#include "systems/base.hpp"

class EntityManager : public Events::Subscriber
{
public:
    typedef Entity::UUID UUID;

    EntityManager();
    ~EntityManager();

    void initialize();
    void destroy();

    std::shared_ptr<Entity> createEntity(const std::string &debugName = "");
    void destroyEntity(UUID uuid);

    template <class T>
    std::shared_ptr<T> getComponent(UUID uuid);

    //void onEvent(const Events::EntityCreated &event);
    void onEvent(const Events::ComponentCreated &event);

	std::string toString() const;

private:
    bool _destroyed;
    boost::uuids::random_generator _generator;	
	
    // Subject to change based on performance considerations
    typedef Systems::Base::ComponentMap<Components::Base> ComponentMap; 
	typedef std::pair<std::shared_ptr<Entity>, ComponentMap>
            EntityComponentsPair;
	typedef std::unordered_map<UUID,
							   EntityComponentsPair,
							   boost::hash<boost::uuids::uuid>> EntityMap;

    EntityMap _map;
};

template <class T>
std::shared_ptr<T> EntityManager::getComponent(UUID uuid)
{
	std::shared_ptr<T> ptr(nullptr);

	EntityComponentsPair *pair;

	// First, see if we have an entity entry that matches uuid
    try
    {
		pair = &_map.at(uuid);
	}
	catch (std::out_of_range &e)
	{
		std::ostringstream ss;
		ss << "no such entity for UUID " << uuid;
		throw Exception(ss.str());
	}

	// Try to find an entry for type T
	auto &componentsMap = pair->second;
	std::shared_ptr<Components::Base> basePtr;
	try
	{
		// should this use move semantics?
		basePtr = componentsMap.at(typeid(T));
	}
	catch (std::out_of_range &e)
	{
		std::ostringstream ss;
		ss << "no entry for component of type "
		   << boost::core::demangle(typeid(T).name())
		   << " in entity " << pair->first;
		throw Exception(ss.str());
	}

	// Make sure the pointer isn't null
	if (!basePtr)
	{
		std::ostringstream ss;
		ss << "entry for component of type "
		   << boost::core::demangle(typeid(T).name())
		   << " in entity " << pair->first << "is null";
		throw Exception(ss.str());
	}

	// Cast to the proper derived type
	ptr = std::move(std::dynamic_pointer_cast<T>(basePtr));
	if (!ptr)
	{
		std::ostringstream ss;
		ss << "could not cast component from Components::Base to "
		   << boost::core::demangle(typeid(T).name()) << " in entity "
		   << pair->first;
		throw Exception(ss.str());
    }

    return ptr;
}

#endif
