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

#include "defines.hpp"
#include "texture.hpp"

#include <cassert>
#include <easylogging++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include "colors.hpp"
#include "exception.hpp"
#include "game.hpp"

#ifdef _TEST_TEXTURE_DIMENSIONS
#   define IS_POWER_OF_TWO(n) ((n & (n - 1)) == 0)
#endif

Texture::Texture(const std::string &name,
                 const std::string &filename) :
    _name(name),
    _texture(0),
    _width(0),
    _height(0)
{
    auto surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::ostringstream ss;
        ss << "unable to load \"" << filename << "\": " << IMG_GetError();
        throw SDLImageException(ss.str());
    }

    copySurfaceToGL(surface);
    SDL_FreeSurface(surface);

    LOG(INFO) << "loaded texture \"" << _name << "\" from file \""
              << filename << "\"";
}

Texture::Texture(const std::string &name,
                 SDL_Surface *surface,
                 bool freeSurface,
                 bool optimize) :
    _name(name),
    _texture(0),
    _width(0),
    _height(0)
{
    if (!surface)
        throw SDLException("null surface passed to Texture constructor");

    copySurfaceToGL(surface, optimize);
    if (freeSurface)
        SDL_FreeSurface(surface);

    LOG(INFO) << "loaded texture \"" << _name << "\" from SDL_Surface";
}

Texture::~Texture()
{
    assert(glIsTexture(_texture));
    glDeleteTextures(1, &_texture);

    LOG(INFO) << "released texture \"" << getName() << "\"";
}

std::string Texture::toString() const
{
    std::ostringstream ss;
    ss << "Texture[name = \"" << getName() << "\", glTexture = "
       << getRawTexture() << "]";
    return ss.str();
}

void Texture::copySurfaceToGL(SDL_Surface *surface,
                              bool optimize)
{
    auto workingSurface = surface;

    // Optimize the surface if requested
    if (optimize)
    {
        auto optimizedSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                     workingSurface->w,
                                                     workingSurface->h,
                                                     32,
                                                     Colors::RMASK,
                                                     Colors::GMASK,
                                                     Colors::BMASK,
                                                     Colors::AMASK);
        if (!optimizedSurface)
            throw SDLException();

        SDL_BlitSurface(workingSurface, NULL, optimizedSurface, NULL);
        workingSurface = optimizedSurface;
    }

    _width = workingSurface->w;
    _height = workingSurface->h;

#ifdef _TEST_TEXTURE_DIMENSIONS
    if (!IS_POWER_OF_TWO(_width))
    {
        LOG(WARNING) << "width of texture \"" << getName()
                     << "\" is not a power of two: " << _width;
    }

    if (!IS_POWER_OF_TWO(_height))
    {
        LOG(WARNING) << "height of texture \"" << getName()
                     << "\" is not a power of two: " << _height;
    }
#endif

    // Make sure the image is either 24 or 32 bpp
    GLenum colorFormat;
    auto numColors = workingSurface->format->BytesPerPixel;
    switch (numColors)
    {
    case 4:
        colorFormat = GL_RGBA;
        break;
    case 3:
        colorFormat = GL_RGB;
        break;

    default:
        {
            std::ostringstream ss;
            ss << "intermediate SDL_Surface for texture \"" << getName()
               << "\" is not truecolor (24 or 32 bpp)";
            throw SDLException(ss.str());
        }
    }

    // Prepare to create an OpenGL texture
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Copy the actual texture data from the SDL_Surface to video memory
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 numColors,
                 _width,
                 _height,
                 0,
                 colorFormat,
                 GL_UNSIGNED_BYTE,
                 workingSurface->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up the optimized surface if necessary (the original working
    // surface will be freed by the caller)
    if (optimize)
        SDL_FreeSurface(workingSurface);
}
