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

std::shared_ptr<Graphics::Texture>
Graphics::Texture::create(const std::string &name,
                          const std::string &filename,
                          SDL_Color *colorKey)
{
    return create(name, loadSurfaceFromFile(filename), nullptr, colorKey);
}

std::shared_ptr<Graphics::Texture>
Graphics::Texture::create(const std::string &name,
                          SDL_Surface *surface,
                          SDL_Rect *rect,
                          SDL_Color *colorKey)
{
    auto texture = std::shared_ptr<Texture>(new Texture(name, surface, rect, colorKey));
    getGame().getTexMgr().add(name, texture);
    return texture;
}

std::shared_ptr<Graphics::Texture>
Graphics::Texture::create(const std::string &name,
                          const Vector2i &dimensions,
                          SDL_Color *color)
{
#if 1
    return create(name, createBlankSurface(dimensions, color));
#else
    auto renderer = getGame().getRenderer()._renderer;
    auto rawTexture = SDL_CreateTexture(renderer,
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        dimensions.x,
                                        dimensions.y);
    if (!rawTexture)
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    // Do a bit of hackery to clear the new texture by setting it as the
    // render destination and changing the screen clear color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderTarget(renderer, rawTexture);
    SDL_RenderClear(renderer);

    // Make sure to set it back
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderTarget(renderer, nullptr);

    auto texture = std::shared_ptr<Texture>(new Texture(name, rawTexture));
    getGame().getTexMgr().add(name, texture);
    return texture;
#endif
}

Graphics::Texture::Texture(const std::string &name,
                           SDL_Surface *surface,
                           SDL_Rect *rect,
                           SDL_Color *colorKey) :
    _name(name),
    _texture(nullptr)
{
    if (!surface)
    {
        throw Exceptions::Base("tried to create Texture from null surface");
    }

    // Set up color keying
    auto color32 = Color::convertToUint32(colorKey ? *colorKey : Color::COLOR_KEY,
                                          surface->format);
    SDL_SetColorKey(surface, true, color32);

    if (rect)
    {
        // Create an intermediate surface for the rect to blit
        auto realSurface = createBlankSurface(Vector2i(rect->w, rect->h));
        SDL_BlitSurface(surface, rect, realSurface, nullptr);
        SDL_FreeSurface(surface);
        surface = realSurface;
    }

    // Create the texture
    _texture = SDL_CreateTextureFromSurface(getGame().getRenderer()._renderer,
                                            surface);
    if (!_texture)
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    // Free the now-unused surface
    SDL_FreeSurface(surface);

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "created texture " << this;
#endif
}

Graphics::Texture::Texture(const std::string &name,
                           SDL_Texture *rawTexture) :
    _name(name),
    _texture(rawTexture)
{
    assert(rawTexture);

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "created texture " << this;
#endif
}

Graphics::Texture::~Texture()
{
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

SDL_Surface *Graphics::Texture::loadSurfaceFromFile(const std::string &filename)
{
    SDL_Surface *s = IMG_Load(filename.c_str());
    if (!s)
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    return s;
}

SDL_Surface *Graphics::Texture::createBlankSurface(const Vector2i &dimensions,
                                                   SDL_Color *color)
{
    auto surface =
            SDL_CreateRGBSurface(0,
                                 dimensions.x,
                                 dimensions.y,
                                 32,
                                 Color::RMASK,
                                 Color::GMASK,
                                 Color::BMASK,
                                 Color::AMASK);

    if (surface == nullptr)
        throw Exceptions::RendererException(SDL_GetError());

    if (!color) color = const_cast<SDL_Color *>(&Color::WHITE);

    SDL_FillRect(surface,
                 nullptr,
                 SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a));

    return surface;
}


