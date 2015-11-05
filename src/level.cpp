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
#include "level.hpp"

#if !defined(_USE_NEW_ENTITY) || !defined(_USE_NEW_COMPONENTS)

#include <iostream>
#include <sstream>
#include <vector>

#include "exception.hpp"
#include "game.hpp"
#include "vector.hpp"

Level::Level(const std::string &filename)
{
}

Level::~Level()
{
    _entities.clear();
}

void Level::update(float dt)
{
    for (auto entity : _entities)
    {
        entity->update(dt);
    }
}

void Level::draw()
{
    for (auto entity : _entities)
    {
        entity->graphics->draw();
    }
}

void Level::cullDeadEntities()
{
    _entities.remove_if([](std::shared_ptr<Entity> e)
    {
        return e->isMarkedForDeath();
    });
}

#endif
