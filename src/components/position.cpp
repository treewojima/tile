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

#if 0
class Position : public Base
{
public:
    // NOTE: Should these be replaced by getters/setters?
    float x, y;

    Position(const Vector2f pos = Vector2f::ZERO);
    Position(float x, float y);
    ~Position();

    inline Vector2f toVector() const { return Vector2f(x, y); }

    std::string toString() const;
};
#endif

Components::Position::Position(const Vector2f pos) :
    Components::Base("position"),
    x(pos.x),
    y(pos.y)
{
#ifdef _DEBUG_COMPONENTS
    LOG(DEBUG) << "created component " << toString();
#endif
}

Components::Position::Position(float x_, float y_) :
    Components::Base("position"),
    x(x_),
    y(y_)
{
#ifdef _DEBUG_COMPONENTS
    LOG(DEBUG) << "created component " << toString();
#endif
}

std::string Components::Position::toString() const
{
    std::ostringstream ss;
    ss << "Components::Position[name = \"" << getName() << "\", "
       << "position = " << Vector2f(x, y) << "]";
    return ss.str();
}
