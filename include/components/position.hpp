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

#ifndef __COMPONENTS_POSITION_HPP__
#define __COMPONENTS_POSITION_HPP__

#include "defines.hpp"
#include "components/base.hpp"
#include "vector.hpp"

namespace Components
{
    class Position : public Base
    {
    public:
        // NOTE: Should these be replaced by getters/setters?
        float x, y;

        Position(const Vector2f pos = Vector2f::ZERO);
        Position(float x_, float y_);

        inline Vector2f toVector() const { return Vector2f(x, y); }

        std::string toString() const;
    };
}

#endif
