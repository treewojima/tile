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

#ifndef __GRAPHICS_WINDOW_HPP__
#define __GRAPHICS_WINDOW_HPP__

#include "defines.hpp"
#include "exceptions.hpp"
#include "vector.hpp"

class SDL_Window;

namespace Graphics
{
    class Window : public Stringable
    {
        friend class Renderer;

    public:
        Window();
        ~Window();

        void destroy();

        void setTitle(const std::string &&title);

        Vector2i getDimensions() const;
        inline int getWidth() const { return getDimensions().x; }
        inline int getHeight() const { return getDimensions().y; }

        std::string toString() const;

    private:
        SDL_Window *_window;
        unsigned _width, _height;
    };
}

namespace Exceptions
{
    class WindowException : public Base
    {
    public:
        using Base::Base;
    };
}

#endif
