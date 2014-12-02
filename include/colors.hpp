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

#ifndef __COLORS_HPP__
#define __COLORS_HPP__

#include "defines.hpp"
#include <SDL2/SDL.h>

namespace Colors
{
    extern const SDL_Color BLACK;
    extern const SDL_Color WHITE;
    extern const SDL_Color RED;
    extern const SDL_Color GREEN;
    extern const SDL_Color BLUE;
    extern const SDL_Color BROWN;
    extern const SDL_Color GRAY;

    SDL_Color makeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
}

#endif
