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

#include "defines.hpp"
#include "entitymanager.hpp"

#include <algorithm>

#include "entity.hpp"
#include "events/dispatcher.hpp"

EntityManager::EntityManager() : _destroyed(true)
{
    //Events::Dispatcher::subscribe<Events::EntityCreated>(*this);
    Events::Dispatcher::subscribe<Events::ComponentCreated>(*this);
}

EntityManager::~EntityManager()
{
    if (!_destroyed) destroy();
}

void EntityManager::initialize()
{
    _destroyed = false;
    //_map.clear();
}

void EntityManager::destroy()
{
	if (_destroyed) return;

    // TODO: update this as necessary for additional cleanup procedures

    //for (auto &pair : _map)
    //{
    //}

    Events::Dispatcher::unsubscribe(*this);
    _map.clear();

    _destroyed = true;
}

std::shared_ptr<Entity> EntityManager::createEntity(const std::string &debugName)
{
    auto entity = std::shared_ptr<Entity>(new Entity(_generator(), debugName));

    EntityComponentsPair pair;
    pair.first = entity;
    _map[entity->getUUID()] = pair;

    Events::Dispatcher::raise<Events::EntityCreated>(entity);

    return entity;
}

void EntityManager::destroyEntity(UUID uuid)
{
    _map.erase(uuid);
}

/*void EntityManager::onEvent(const Events::EntityCreated &event)
{
    EntityComponentsPair pair;
    pair.first = event.entity;

    _map[event.entity->getUUID()] = pair;
}*/

void EntityManager::onEvent(const Events::ComponentCreated &event)
{
    auto component = event.component;

    // TODO: Should this have error checking in case the entity doesn't exist?
    //_map[component->getParent()->getUUID()].second.push_back(component);
	_map[component->getParent()->getUUID()].second[typeid(*component)] = component;
}

std::string EntityManager::toString() const
{
	std::ostringstream ss;
	ss << "EntityManager[entityCount = " << _map.size() << "]";
	return ss.str();
}
