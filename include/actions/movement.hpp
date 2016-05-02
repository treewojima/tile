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

#ifndef __ACTIONS_MOVEMENT_HPP__
#define __ACTIONS_MOVEMENT_HPP__

#include "actions/base.hpp"
//#include "direction.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include <memory>

namespace Actions
{
	class Movement : public Base
    {
	public:
        //Movement(const Vector2i &origin, const Vector2i &dest);
        Movement(std::shared_ptr<const Entity> entity_,
                 const Vector2i &&origin_,
                 const Vector2i &&dest_);

		std::string toString() const;

        std::shared_ptr<const Entity> entity;
        const Vector2i origin, dest;
        Vector2i current;
	};
}

#endif
