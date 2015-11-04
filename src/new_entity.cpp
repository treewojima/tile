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
#include "entity.hpp"

#ifdef _USE_NEW_ENTITY

#include <sstream>

#include "entitymanager.hpp"
#include "events/dispatcher.hpp"
#include "game.hpp"
#include "logger.hpp"

std::shared_ptr<Entity> Entity::create(const std::string &debugName)
{
    return Game::getEntityMgr().createEntity(debugName);
}

Entity::Entity(UUID uuid, const std::string &debugName) :
    _uuid(uuid),
    _debugName(debugName)
{
#ifdef _DEBUG_ENTITIES
    LOG_DEBUG << "created entity " << toString();
#endif
}

Entity::~Entity()
{
#ifdef _DEBUG_ENTITIES
    LOG_DEBUG << "destroyed entity " << toString();
#endif
}

std::string Entity::toString() const
{
    std::ostringstream ss;
    ss << "Entity[uuid = " << _uuid << ", debugName = \"" << _debugName << "\"]";
    return ss.str();
}

#endif
