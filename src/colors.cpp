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

#include <sstream>

#include "exceptions.hpp"
#include "logger.hpp"

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

SDL_Color Colors::makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return MAKE_COLOR(r, g, b, a);
}

SDL_Color Colors::makeColor(uint32_t c)
{
    return makeColor(c >> 24,
                     c >> 16,
                     c >> 8,
                     c);
}

SDL_Color Colors::parseRGBHexString(std::string s)
{
    // Passed in as a 24-bit color w/o alpha
    if (s.size() == 6)
    {
        s += "FF";
    }

    if (s.size() != 8)
    {
        std::ostringstream ss;
        ss << "Unable to parse hex color string \"" << s << "\"";
        throw Exceptions::Base(s);
    }

    uint32_t c;
    std::stringstream stream;

    stream << std::hex << s;
    stream >> c;

    return makeColor(c);
}

uint32_t Colors::convertToUint32(const SDL_Color &color,
                                 const SDL_PixelFormat *format)
{
    return SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
}
