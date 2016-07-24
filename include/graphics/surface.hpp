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

#ifndef __GRAPHICS_SURFACE_HPP__
#define __GRAPHICS_SURFACE_HPP__

#include "defines.hpp"
#include "color.hpp"
#include "exceptions.hpp"
#include "stringable.hpp"
#include "vector.hpp"

namespace Graphics
{
    class Surface : public Stringable
    {
        friend class Texture;

    public:
        static Surface *create(SDL_Surface *surface,
                               SDL_Rect *rect = nullptr,
                               const SDL_Color &colorKey = Color::COLOR_KEY);
        static Surface *create(const Vector2i &dimensions,
                               const SDL_Color &color = Color::WHITE);
        static Surface *create(const std::string &filename,
                               const SDL_Color &colorKey = Color::COLOR_KEY);

    private:
        Surface(SDL_Surface *surface,
                SDL_Rect *rect,
                const SDL_Color &colorKey);

    public:
        ~Surface();

        static void blit(Surface *src,
                         SDL_Rect *srcRect,
                         Surface *dest,
                         SDL_Rect *destRect);

        inline unsigned getWidth() const { return _surface->w; }
        inline unsigned getHeight() const { return _surface->h; }
        inline Vector2i getDimensions() const { return Vector2i(getWidth(), getHeight()); }

        std::string toString() const;

    private:
        SDL_Surface *_surface;

        static void innerBlit(SDL_Surface *src,
                              SDL_Rect *srcRect,
                              SDL_Surface *dest,
                              SDL_Rect *destRect);

        static SDL_Surface *createBlankSurface(const Vector2i &dimensions,
                                               const SDL_Color &color = Color::WHITE);
    };
}

namespace Exceptions
{
    // Gotta stick this somewhere
    class GraphicsException : public Base
    {
    public:
        GraphicsException(const std::string &s) : Base(s) {}
        GraphicsException(const char *s) : Base(s) {}
        GraphicsException() : Base(SDL_GetError()) {}
    };
}

#endif
