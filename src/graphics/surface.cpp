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

#include "graphics/surface.hpp"

#include <cassert>
#include <SDL2/SDL_image.h>

std::shared_ptr<Graphics::Surface>
Graphics::Surface::create(SDL_Surface *surface,
                          SDL_Rect *rect,
                          const SDL_Color &colorKey)
{
    return std::shared_ptr<Surface>(new Surface(surface, rect, colorKey));
}

std::shared_ptr<Graphics::Surface>
Graphics::Surface::create(const Vector2i &dimensions, const SDL_Color &color)
{
    return create(createBlankSurface(dimensions, color));
}

std::shared_ptr<Graphics::Surface>
Graphics::Surface::create(const std::string &filename,
                          const SDL_Color &colorKey)
{
    auto surface = IMG_Load(filename.c_str());
    if (!surface)
        throw Exceptions::GraphicsException();
    return create(surface, colorKey);
}

Graphics::Surface::Surface(SDL_Surface *surface,
                           SDL_Surface *rect,
                           const SDL_Color &colorKey)
{
    if (!surface)
    {
        throw Exceptions::GraphicsException("tried to create Surface from null source");
    }

    if (rect)
    {
        auto newSurface = createBlankSurface(Vector2i(rect->w, rect->h));
        SDL_BlitSurface(surface, rect, newSurface, nullptr);
        SDL_FreeSurface(surface);
        surface = newSurface;
    }

    SDL_SetColorKey(surface, true, Color::toUnsigned(colorKey, surface->format));

    _surface = surface;
}

Graphics::Surface::~Surface()
{
    destroy();
}

void Graphics::Surface::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    assert(_surface);
    SDL_FreeSurface(_surface);

    destroyed = true;
}

std::string Graphics::Surface::toString() const
{
    std::ostringstream ss;
    ss << "Surface[dimensions = " << getDimensions() << "]";
    return ss.str();
}

SDL_Surface *Graphics::Surface::createBlankSurface(const Vector2i &dimensions,
                                                   const SDL_Color &color)
{
    auto surface = SDL_CreateRGBSurface(0,
                                        dimensions.x,
                                        dimensions.y,
                                        32,
                                        Color::RMASK,
                                        Color::GMASK,
                                        Color::BMASK,
                                        Color::AMASK);
    if (!surface)
        throw Exceptions::GraphicsException();

    SDL_FillRect(surface, nullptr, Color::toUnsigned(color, surface->format));

    return surface;
}
