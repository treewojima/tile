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

#include "actions/movement.hpp"

#include <sstream>

#include "logger.hpp"

#if 0
Actions::Movement::Movement(const Vector2i &origin, const Vector2i &dest) :
    _origin(origin),
    _dest(dest),
    _current()
{
}
#endif

Actions::Movement::Movement(std::shared_ptr<const Entity> entity_,
                            const Vector2i &&origin_,
                            const Vector2i &&dest_) :
    entity(entity_),
    origin(std::move(origin_)),
    dest(std::move(dest_)),
    current(origin_)
{
    if (origin == dest)
    {
        LOG_WARNING << *this << " will have a net movement of zero";
    }
}

std::string Actions::Movement::toString() const
{
    std::ostringstream ss;
    ss << "Actions::Movement[entity = " << entity << ", "
       << "origin = " << origin << ", "
       << "current = " << current << ", "
       << "dest = " << dest << "]";
    return ss.str();
}
