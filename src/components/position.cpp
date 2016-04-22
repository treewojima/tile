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
#include "components/position.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"
#include "map.hpp"
#include "window.hpp"

Components::Position::Position(std::shared_ptr<Entity> parent,
                               float x_,
                               float y_) :
    Base(parent, parent->getDebugName() + "Position"),
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

std::shared_ptr<Components::Position>
Components::Position::create(std::shared_ptr<Entity> parent,
                             float x,
                             float y)
{
    auto ptr = std::shared_ptr<Position>(new Position(parent, x, y));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::PositionComponentCreated>(ptr);
    return ptr;
}

std::shared_ptr<Components::Position>
Components::Position::create(std::shared_ptr<Entity> parent,
                             const Vector2f &v)
{
    return create(parent, v.x, v.y);
}

std::shared_ptr<Components::Position>
Components::Position::create(std::shared_ptr<Entity> parent,
							 const Components::MapPosition &mapPosition)
{
    float x = (mapPosition.x + 1) * Map::TILE_WIDTH - (Map::TILE_WIDTH / 2);
    float y = (mapPosition.y + 1) * Map::TILE_HEIGHT - (Map::TILE_HEIGHT / 2);
	return create(parent, x, y);
}
