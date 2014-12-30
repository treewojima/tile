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

void Window::blitTexture(TextureManager::ConstResourcePtr texture,
                         float x,
                         float y)
{
    const auto width = texture->getWidth();
    const auto height = texture->getHeight();
    const float centeredWidth = width / 2.f;
    const float centeredHeight = height / 2.f;

    // NOTE: Should there be a glLoadIdentity() here as well?

    glBindTexture(GL_TEXTURE_2D, texture->getRawTexture());
    glTranslatef(x - centeredWidth, y - centeredHeight, 0);
    glBegin(GL_QUADS);
        glTexCoord2i(1, 1);
        glVertex2f(width, 0);
        glTexCoord2i(0, 1);
        glVertex2f(0, 0);
        glTexCoord2i(0, 0);
        glVertex2f(0, height);
        glTexCoord2i(1, 0);
        glVertex2f(width, height);
    glEnd();

    glLoadIdentity();
}

void Window::blitTexture(TextureManager::ConstResourcePtr texture,
                         const Vector2f &pos)
{
    blitTexture(texture, pos.x, pos.y);
}

void Window::blitTexture(TextureManager::ConstResourcePtr texture,
                         const Vector2i &pos)
{
    blitTexture(texture, pos.x, pos.y);
}

void Window::flip()
{
    SDL_GL_SwapWindow(_window);
}

void Window::setTitle(const std::string &title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}
