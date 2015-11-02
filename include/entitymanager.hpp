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
#include <map>

#include "components/base.hpp"
#include "entity.hpp"
#include "events/subscriber.hpp"
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

private:
    bool _destroyed;
    boost::uuids::random_generator _generator;

    // Subject to change based on performance considerations
    typedef Systems::Base::ComponentList<Components::Base> ComponentList;
    typedef std::pair<std::shared_ptr<Entity>, ComponentList>
            EntityComponentsPair;
    typedef std::map<UUID, EntityComponentsPair> EntityMap;

    EntityMap _map;
};

template <class T>
std::shared_ptr<T> EntityManager::getComponent(UUID uuid)
{
    try
    {
        // This is an absolutely HORRIBLE, UGLY, HIDEOUSLY NAIVE FUNCTION.
        // FUCKING FIX THIS.

        auto &pair = _map.at(uuid);
        auto &components = pair.second;
        auto iter = std::find_if(components.begin(),
                                 components.end(),
                                 [](const std::shared_ptr<Components::Base> &value)
                                 { return dynamic_cast<T *>(value.get()) != nullptr; });
        if (iter != components.end())
        {
            return std::dynamic_pointer_cast<T>(*iter);
        }
    }
    catch (std::out_of_range &e)
    {
        // Just fall through
    }

    return std::shared_ptr<T>(nullptr);
}

#endif
