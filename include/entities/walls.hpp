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

#ifndef __WALLS_HPP__
#define __WALLS_HPP__

#include "defines.hpp"
#include "entity.hpp"

class b2Body;

class Walls : public Entity
{
    friend class Paddle;
    friend class Ball;

public:
    Walls();
    ~Walls();

    void draw() {}

private:
    b2Body *_body;
    b2Fixture *_bottomFixture;
};

#endif
