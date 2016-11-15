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

#ifndef __COMPONENTS_DEBUGINFO_HPP__
#define __COMPONENTS_DEBUGINFO_HPP__

#include "defines.hpp"
#include "entity.hpp"
#include "game.hpp"

namespace Components
{
    // This class serves as a grab-bag for properties that are only useful for
    // debugging purposes, or that will be ultimately refactored into other
    // components
    class DebugInfo : public Base
    {
    public:
        static DebugInfo *create(const Entity::UUID &parent,
                                 const std::string &name);

    private:
        DebugInfo(const Entity::UUID &parent,
                  const std::string &name_);

    public:
        std::string name;

        std::string toString() const;
    };
}

namespace Events
{
    typedef SpecificComponentCreated<Components::DebugInfo>
        DebugInfoComponentCreated;
}

#endif
