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

#ifndef __HELPER_EVENTS_HPP__
#define __HELPER_EVENTS_HPP__

#include "defines.hpp"
#include "events/base.hpp"
#include "vector.hpp"

namespace Events
{
    // SDL quit event, or any keypress tied to it
    class Quit : public Base {};

    // Mouse down
    class MouseDown : public Base
    {
    public:
        enum class Button { Right, Left } button;
        Vector2i position;

        MouseDown(Button b, Vector2i pos) : button(b), position(pos) {}
    };

    // Key down
    class KeyDown : public Base
    {
    public:
        static const uint8_t *keys;

        KeyDown() {}
    };
}

#endif
