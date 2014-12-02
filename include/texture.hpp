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

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "defines.hpp"
#include <SDL2/SDL.h>
#include <string>
#include "exception.hpp"

#ifdef _ENABLE_TEXTURES

class Texture
{
public:
    Texture(const std::string &filename);
    ~Texture();

    void load(const std::string &filename);

    inline SDL_Texture *getRawTexture() const { return _texture; }

    inline int getWidth() const
    {
        int w = 0;
        if (SDL_QueryTexture(getRawTexture(), nullptr, nullptr, &w, nullptr))
            throw SDLException();
        return w;
    }

    inline int getHeight() const
    {
        int h = 0;
        if (SDL_QueryTexture(getRawTexture(), nullptr, nullptr, nullptr, &h))
            throw SDLException();
        return h;
    }

private:
    SDL_Texture *_texture;
};

#endif

#endif
