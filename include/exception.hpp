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
#ifdef _ENABLE_AUDIO
#   include <SDL2/SDL_mixer.h>
#endif
#include <sstream>
#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};



#ifdef _ENABLE_AUDIO
#error move this class to another file
class SDLMixerException : public SDLException
{
public:
    SDLMixerException(const std::string &msg) : SDLException(msg) {}
    SDLMixerException() : SDLException(Mix_GetError()) {}
};
#endif





#endif
