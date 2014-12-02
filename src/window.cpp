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

#include "window.hpp"

#include <cassert>
#include <easylogging++.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "exception.hpp"
#include "game.hpp"

// Locals
namespace
{
    SDL_Window *_window;
    SDL_GLContext _glContext;
}

void Window::create(int width, int height)
{
    assert(width > 0);
    assert(height > 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    _window = SDL_CreateWindow("sdl",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width,
                               height,
                               SDL_WINDOW_OPENGL);
    if (_window == nullptr)
        throw SDLException();

    LOG(INFO) << "created window (width = " << width
              << ", height = " << height << ")";

    _glContext = SDL_GL_CreateContext(_window);
    if (!_glContext)
    {
        SDL_DestroyWindow(_window);
        throw SDLException();
    }
}

void Window::destroy()
{
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);

    LOG(INFO) << "destroyed window";
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

/*void Window::blitTexture(const Texture &texture, int x, int y)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = texture.getWidth();
    r.h = texture.getHeight();

    SDL_RenderCopy(_renderer,
                   texture.getRawTexture(),
                   nullptr,
                   &r);
}*/

void Window::flip()
{
    SDL_GL_SwapWindow(_window);
}

void Window::setTitle(const std::string &title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}

#if 0
float Window::getWidth()
{
    Game::getCamera().
    return getWidthPixels() * Physics::PIXELS_TO_METERS;
}

float Window::getHeight()
{
    return getHeightPixels() * Physics::PIXELS_TO_METERS;
}

int Window::getWidthPixels()
{
    int w;
    SDL_GetWindowSize(_window, &w, nullptr);
    return w;
}

int Window::getHeightPixels()
{
    int h;
    SDL_GetWindowSize(_window, nullptr, &h);
    return h;
}
#endif
