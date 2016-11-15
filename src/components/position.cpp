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

#include "defines.hpp"
#include "components/position.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"
#include "map/map.hpp"

Components::Position
*Components::Position::create(const Entity::UUID &parent,
                              int x,
                              int y,
                              const std::string &debugName)
{
    auto ptr = new Position(parent, x, y, debugName);
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::PositionComponentCreated>(ptr);
    return ptr;
}

Components::Position
*Components::Position::create(const Entity::UUID &parent,
                              const Vector2i &v,
                              const std::string &debugName)
{
    return create(parent, v.x, v.y, debugName);
}

Components::Position
*Components::Position::create(const Entity::UUID &parent,
                              const Components::MapPosition &mapPosition,
                              const std::string &debugName)
{
    int x = (mapPosition.x + 1) * Map::Map::TILE_WIDTH - (Map::Map::TILE_WIDTH / 2);
    int y = (mapPosition.y + 1) * Map::Map::TILE_HEIGHT - (Map::Map::TILE_HEIGHT / 2);
    return create(parent, x, y, debugName);
}

Components::Position::Position(const Entity::UUID &parent,
                               int x_,
                               int y_,
                               const std::string &debugName) :
    Base(parent, debugName),
    x(x_),
    y(y_)
{
}

std::string Components::Position::toString() const
{
    std::ostringstream ss;
    ss << "Components::Position[debugName = \"" << getDebugName() << "\", "
       << "parent = " << getParent() << "\", "
       << "position = " << toVector() << "]";
    return ss.str();
}


