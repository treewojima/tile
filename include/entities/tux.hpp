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

#ifndef __TUX_HPP__
#define __TUX_HPP__

#include "defines.hpp"

#ifndef _USE_NEW_ENTITY

#include "entity.hpp"

#include <vector>

#include "game.hpp"

class Tux : public Entity
{
public:
    Tux();
    ~Tux();

private:
    std::vector<Game::Event::Handle> _eventHandles;
};

#endif

#endif
