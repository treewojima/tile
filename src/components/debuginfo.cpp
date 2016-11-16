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

#include "components/debuginfo.hpp"
#include "events/dispatcher.hpp"

Components::DebugInfo
*Components::DebugInfo::create(const Entity::UUID &parent,
                               const std::string &name)
{
    auto ptr = new DebugInfo(parent, name);
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::DebugInfoComponentCreated>(ptr);
    return ptr;
}

Components::DebugInfo::DebugInfo(const Entity::UUID &parent,
                                 const std::string &name_) :
    Base(parent, name_),
    name(name_)
{
}

std::string Components::DebugInfo::toString() const
{
    std::ostringstream ss;
    ss << "Components::DebugInfo[parent = " << getParent()
       << ", name = " << name << "]";
    return ss.str();
}
