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

#include <cassert>
#include <cmath>
#include <easylogging++.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <sstream>

#include "physics.hpp"

Entity::Entity(const std::string &name, Type type) :
    _name(name),
    _type(type),
    _markedForDeath(false)
{
}

Entity::~Entity()
{
    LOG(DEBUG) << "destroyed entity " << getName();
}

std::string Entity::toString() const
{
    std::ostringstream ss;
    ss << "Entity[name = \"" << getName() << "\"]";
    return ss.str();
}
