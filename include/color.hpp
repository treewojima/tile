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

#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include "defines.hpp"
#include <cstdint>
#include <SDL2/SDL.h>
#include <string>

namespace Color
{
    extern const SDL_Color BLACK;
    extern const SDL_Color WHITE;
    extern const SDL_Color RED;
    extern const SDL_Color GREEN;
    extern const SDL_Color BLUE;
    extern const SDL_Color BROWN;
    extern const SDL_Color GRAY;

    extern const SDL_Color COLOR_KEY;

    extern const uint32_t RMASK;
    extern const uint32_t GMASK;
    extern const uint32_t BMASK;
    extern const uint32_t AMASK;

    SDL_Color makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    SDL_Color makeColor(uint32_t c);

    SDL_Color parseRGBHexString(std::string s);

    uint32_t toUnsigned(const SDL_Color &color,
                        const SDL_PixelFormat *format);
}

#endif
