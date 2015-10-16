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
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "exception.hpp"
#include "game.hpp"
#include "logger.hpp"

// Locals
namespace
{
    SDL_Window *_window;
    SDL_GLContext _glContext;
}

void Window::create(int width, int height, bool vsync)
{
    assert(width > 0);
    assert(height > 0);

	// Double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create the window
    _window = SDL_CreateWindow("sdl",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width,
                               height,
                               SDL_WINDOW_OPENGL);
    if (_window == nullptr)
        throw SDLException();

    LOG_INFO << "created window (width = " << width
             << ", height = " << height << ")";

	// Create the OpenGL context
    _glContext = SDL_GL_CreateContext(_window);
    if (!_glContext)
    {
        SDL_DestroyWindow(_window);
        throw SDLException();
    }

	if (vsync)
	{
		// First, try to enable late swap tearing
		if (SDL_GL_SetSwapInterval(-1) < 0)
		{
			// Late swap tearing isn't supported, so fall back onto normal vsync
			if (SDL_GL_SetSwapInterval(1) < 0)
			{
				// Couldn't enable vsync
				throw SDLException();
			}

			LOG_INFO << "vsync enabled";
		}
		else
		{
			LOG_INFO << "vsync with late swap tearing enabled";
		}
	}
	else
	{
		if (SDL_GL_SetSwapInterval(0) < 0)
			throw SDLException();
		LOG_INFO << "vsync disabled";
	}
}

void Window::destroy()
{
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);

    LOG_INFO << "destroyed window";
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::flip()
{
    SDL_GL_SwapWindow(_window);
}

void Window::setTitle(const std::string &title)
{
    SDL_SetWindowTitle(_window, title.c_str());
}

Vector2i Window::getDimensions()
{
    Vector2i ret;
    SDL_GetWindowSize(_window, &ret.x, &ret.y);
    return ret;
}
