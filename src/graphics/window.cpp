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

#include "graphics/window.hpp"

#include <cassert>
#include <SDL2/SDL.h>

#include "game.hpp"
#include "logger.hpp"

Graphics::Window::Window()
{
    auto options = getGame().getOptions();
    _width = options.windowWidth;
    _height = options.windowHeight;

    _window = SDL_CreateWindow("sdl",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               _width,
                               _height,
                               SDL_WINDOW_SHOWN);
    if (!_window)
    {
        throw Exceptions::WindowException(SDL_GetError());
    }

    LOG_INFO << "created window " << this;
}

Graphics::Window::~Window()
{
    destroy();
}

void Graphics::Window::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    SDL_DestroyWindow(_window);

    LOG_DEBUG << "destroyed window";
    destroyed = true;
}

void Graphics::Window::setTitle(const std::string &&title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}

Vector2i Graphics::Window::getDimensions() const
{
    Vector2i ret;
    SDL_GetWindowSize(_window, &ret.x, &ret.y);
    return ret;
}

std::string Graphics::Window::toString() const
{
    std::ostringstream ss;
    ss << "Window[dimensions = " << getDimensions() << "]";
    return ss.str();
}
