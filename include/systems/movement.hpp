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

#ifndef __SYSTEMS_MOVEMENT_HPP__
#define __SYSTEMS_MOVEMENT_HPP__

#include "defines.hpp"

#include <map>
#include <queue>

#include "actions/movement.hpp"
#include "systems/base.hpp"
#include "timer.hpp"

namespace Systems
{
    class Movement : public Base
    {
    public:
        Movement();
        ~Movement();

        void update(float dt);
        void destroy();

        void queueMovement(Actions::Movement &&movement);
        void dumpQueue() const;

        std::string toString() const;

    private:
        typedef std::queue<Actions::Movement> MovementQueue;
        typedef std::map<Entity::UUID, MovementQueue> EntityMovementMap;

        EntityMovementMap _movements;
        Timer _timer;
    };
}

#endif
