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
#include "texture.hpp"

#ifdef _ENABLE_TEXTURES

#include <easylogging++.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include "exception.hpp"
#include "game.hpp"

Texture::Texture(const std::string &filename)
{
    load(filename);
}

Texture::~Texture()
{
    LOG(DEBUG) << __FUNCTION__;

    SDL_DestroyTexture(_texture);
}

void Texture::load(const std::string &filename)
{
    auto surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::ostringstream ss;
        ss << "unable to load \"" << filename << "\":" << IMG_GetError();
        throw SDLImageException(ss.str());
    }

    auto texture = SDL_CreateTextureFromSurface(Window::getRenderer(),
                                                surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr)
    {
        std::ostringstream ss;
        ss << "unable to load \"" << filename << "\":" << SDL_GetError();
        throw SDLException(ss.str());
    }
    else
    {
        _texture = texture;
    }
}

#endif
