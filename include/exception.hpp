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

#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include "defines.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class SDLException : public Exception
{
public:
    SDLException(const std::string &msg) : Exception(msg) {}
    SDLException() : Exception(SDL_GetError()) {}
};

class SDLImageException : public SDLException
{
public:
    SDLImageException(const std::string &msg) : SDLException(msg) {}
    SDLImageException() : SDLException(IMG_GetError()) {}
};

class GLException : public Exception
{
public:
    GLException(const std::string &msg) : Exception(msg) {}
    explicit GLException(GLenum error) : Exception(getErrorString(error)) {}

private:
    std::string getErrorString(GLenum error)
    {
        std::ostringstream ss;
        ss << gluErrorString(error);
        return ss.str();
    }
};

class GLEWException : public GLException
{
public:
    GLEWException(const std::string &msg) : GLException(msg) {}
    explicit GLEWException(GLenum error) : GLException(getErrorString(error)) {}

private:
    std::string getErrorString(GLenum error)
    {
        std::ostringstream ss;
        ss << glewGetErrorString(error);
        return ss.str();
    }
};

class PhysicsException : public Exception
{
public:
    using Exception::Exception;
};

#endif
