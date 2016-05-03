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

#ifndef __COMPONENTS_POSITION_HPP__
#define __COMPONENTS_POSITION_HPP__

#include "defines.hpp"

#include "components/base.hpp"
#include "components/mapposition.hpp"
#include "events/base.hpp"
#include "vector.hpp"

namespace Components
{
    class Position : public Base
    {
    private:
        Position(std::shared_ptr<Entity> parent,
                 int x_,
                 int y_);

    public:
        int x, y;

        inline Vector2i toVector() const { return Vector2i(x, y); }

        std::string toString() const;

        static std::shared_ptr<Position> create(std::shared_ptr<Entity> parent,
                                                int x,
                                                int y);
        static std::shared_ptr<Position> create(std::shared_ptr<Entity> parent,
                                                const Vector2i &v = Vector2i::ZERO);
		static std::shared_ptr<Position> create(std::shared_ptr<Entity> parent,
												const MapPosition &position);
	};
}

namespace Events
{
    typedef SpecificComponentCreated<Components::Position> PositionComponentCreated;
}

#endif
