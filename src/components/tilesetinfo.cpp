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

#include "components/tilesetinfo.hpp"
#include "events/dispatcher.hpp"

std::shared_ptr<Components::TilesetInfo>
Components::TilesetInfo::create(const Entity::UUID &parent,
                                int gid_,
                                const std::string &debugName)
{
    auto ptr = std::shared_ptr<TilesetInfo>(new TilesetInfo(parent, gid_, debugName));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::TilesetInfoComponentCreated>(ptr);
    return ptr;
}

Components::TilesetInfo::TilesetInfo(const Entity::UUID &parent,
                                     int gid_,
                                     const std::string &debugName) :
    Base(parent, debugName),
    gid(gid_)
{
}

std::string Components::TilesetInfo::toString() const
{
    std::ostringstream ss;
    ss << "Components::TilesetInfo[parent = " << getParent()
       << ", gid = " << gid << "]";
    return ss.str();
}
