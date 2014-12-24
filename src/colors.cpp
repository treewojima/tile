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
#include "colors.hpp"

#define MAKE_COLOR(r, g, b, a) \
    SDL_Color { (r), (g), (b), (a) }

const SDL_Color Colors::BLACK = MAKE_COLOR(  0,   0,   0, 255);
const SDL_Color Colors::WHITE = MAKE_COLOR(255, 255, 255, 255);
const SDL_Color Colors::RED   = MAKE_COLOR(255,   0,   0, 255);
const SDL_Color Colors::GREEN = MAKE_COLOR(  0, 255,   0, 255);
const SDL_Color Colors::BLUE  = MAKE_COLOR(  0,   0, 255, 255);
const SDL_Color Colors::BROWN = MAKE_COLOR( 64,  13,  18, 255);
const SDL_Color Colors::GRAY  = MAKE_COLOR( 70,  70,  70, 255);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const uint32_t Colors::RMASK = 0xff000000;
const uint32_t Colors::GMASK = 0x00ff0000;
const uint32_t Colors::BMASK = 0x0000ff00;
const uint32_t Colors::AMASK = 0x000000ff;
#else
const uint32_t Colors::RMASK = 0x000000ff;
const uint32_t Colors::GMASK = 0x0000ff00;
const uint32_t Colors::BMASK = 0x00ff0000;
const uint32_t Colors::AMASK = 0xff000000;
#endif

SDL_Color Colors::makeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return MAKE_COLOR(r, g, b, a);
}
