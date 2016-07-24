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

#include "graphics/texture.hpp"

#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

#include "color.hpp"
#include "exceptions.hpp"
#include "game.hpp"
#include "logger.hpp"

#ifdef _TEST_TEXTURE_DIMENSIONS
#   define IS_POWER_OF_TWO(n) ((n & (n - 1)) == 0)
#else
#   define IS_POWER_OF_TWO(n) true
#endif

Graphics::Texture
*Graphics::Texture::create(const std::string &name,
                           const Surface *surface)
{
    auto texture = new Texture(name, surface);
    getGame().getTexMgr().add(name, texture);
    return texture;
}

Graphics::Texture
*Graphics::Texture::create(const std::string &name,
                           const std::string &filename,
                           const SDL_Color &colorKey)
{
    return create(name, Surface::create(filename, colorKey));
}

Graphics::Texture
*Graphics::Texture::create(const std::string &name,
                           const Vector2i &dimensions,
                           const SDL_Color &color)
{
    return create(name, Surface::create(dimensions, color));
}

Graphics::Texture::Texture(const std::string &name,
                           const Surface *surface) :
    _name(name),
    _texture(nullptr)
{
    if (!surface)
    {
        throw Exceptions::GraphicsException("tried to create Texture from null Surface");
    }

    // Create the texture
    _texture = SDL_CreateTextureFromSurface(getGame().getRenderer()._renderer,
                                            surface->_surface);
    if (!_texture)
    {
        throw Exceptions::GraphicsException();
    }

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "created texture " << this;
#endif
}

Graphics::Texture::~Texture()
{
    //getGame().getTexMgr().remove(getName());

    assert(_texture);
    SDL_DestroyTexture(_texture);

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "released texture \"" << this << "\"";
#endif
}

Vector2i Graphics::Texture::getDimensions() const
{
    Vector2i v;
    SDL_QueryTexture(_texture, nullptr, nullptr, &v.x, &v.y);
    return v;
}

std::string Graphics::Texture::toString() const
{
    std::ostringstream ss;
    ss << "Texture[name = \"" << getName() << "\"]";
    return ss.str();
}
