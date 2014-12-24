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

#include <easylogging++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include "colors.hpp"
#include "exception.hpp"
#include "game.hpp"

#define IS_POWER_OF_TWO(n) ((n & (n - 1)) == 0)

Texture::Texture(const std::string &filename) :
    _texture(0),
    _width(0),
    _height(0)
{
    load(filename);
}

Texture::Texture(SDL_Surface *surface, bool freeSurface, bool optimize) :
    _texture(0),
    _width(0),
    _height(0)
{
    if (!surface)
        throw SDLException("null surface passed to Texture constructor");

    _texture = copySurfaceToGL(surface, optimize, &_width, &_height);

    if (freeSurface)
        SDL_FreeSurface(surface);
}

Texture::~Texture()
{
    LOG(DEBUG) << __FUNCTION__;

    glDeleteTextures(1, &_texture);
}

void Texture::load(const std::string &filename)
{
    auto surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::ostringstream ss;
        ss << "unable to load \"" << filename << "\":" << IMG_GetError();
        throw SDLImageException(ss.str());
    }

    // TODO: There's currently no error checking for an invalid GL texture!
    _texture = copySurfaceToGL(surface, true, &_width, &_height);

    SDL_FreeSurface(surface);
}

std::string Texture::toString() const
{
    std::ostringstream ss;
    ss << "Texture[glTexture = " << _texture << "]";
    return ss.str();
}

GLuint Texture::copySurfaceToGL(SDL_Surface *surface,
                                bool optimize,
                                int *outWidth,
                                int *outHeight)
{
    SDL_Surface *workingSurface = surface;

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

    if (IS_POWER_OF_TWO(workingSurface->w))
    {
        LOG(WARNING) << "width of texture is not a power of two: "
                  << workingSurface->w;
    }

    if (IS_POWER_OF_TWO(workingSurface->h))
    {
        LOG(WARNING) << "height of texture is not a power of two: "
                  << workingSurface->h;
    }

    if (outWidth) *outWidth = workingSurface->w;
    if (outHeight) *outHeight = workingSurface->h;

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
        throw SDLException("Surface is not truecolor");
    }

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 numColors,
                 workingSurface->w,
                 workingSurface->h,
                 0,
                 colorFormat,
                 GL_UNSIGNED_BYTE,
                 workingSurface->pixels);

    if (optimize)
        SDL_FreeSurface(workingSurface);

    glGenerateMipmap(GL_TEXTURE_2D);

    return glTexture;
}
