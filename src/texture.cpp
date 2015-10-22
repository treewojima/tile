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

#include <boost/log/trivial.hpp>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

#include "colors.hpp"
#include "exception.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "logger.hpp"

#ifdef _TEST_TEXTURE_DIMENSIONS
#   define IS_POWER_OF_TWO(n) ((n & (n - 1)) == 0)
#else
#   define IS_POWER_OF_TWO(n) true
#endif

Texture::Texture(const std::string &name,
                 const std::string &filename,
                 const SDL_Color *colorKey) :
    _name(name),
    _texture(0),
    _width(0),
    _height(0)
{
    SDL_Surface *surface = Graphics::loadSDLSurface(filename);

    copySurfaceToGL(surface, colorKey);
    SDL_FreeSurface(surface);

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "loaded texture \"" << _name << "\" from file \""
             << filename << "\"";
#endif
}

Texture::Texture(const std::string &name,
                 SDL_Surface *surface,
                 const SDL_Color *colorKey,
                 bool freeSurface,
                 bool optimize) :
    _name(name),
    _texture(0),
    _width(0),
    _height(0)
{
    if (!surface)
        throw SDLException("null surface passed to Texture constructor");

    copySurfaceToGL(surface, colorKey, optimize);
    if (freeSurface)
        SDL_FreeSurface(surface);

#ifdef _DEBUG_TEXTURES
    LOG_INFO << "loaded texture \"" << _name << "\" from SDL_Surface";
#endif
}

Texture::~Texture()
{
    assert(glIsTexture(_texture));
    glDeleteTextures(1, &_texture);

#ifdef _DEBUG_TEXTURES
	LOG_INFO << "released texture \"" << getName() << "\"";
#endif
}

std::string Texture::toString() const
{
    std::ostringstream ss;
    ss << "Texture[name = \"" << getName() << "\", glTexture = "
       << getRawTexture() << "]";
    return ss.str();
}

void Texture::copySurfaceToGL(SDL_Surface *surface,
                              const SDL_Color *colorKey,
                              bool optimize)
{
    auto workingSurface = surface;

    // Optimize the surface if requested
    if (optimize) workingSurface = Graphics::optimizeSDLSurface(workingSurface);

    _width = workingSurface->w;
    _height = workingSurface->h;

    if (!IS_POWER_OF_TWO(_width))
    {
        LOG_WARNING << "width of texture \"" << getName()
                    << "\" is not a power of two: " << _width;
    }

    if (!IS_POWER_OF_TWO(_height))
    {
        LOG_WARNING << "height of texture \"" << getName()
                    << "\" is not a power of two: " << _height;
    }

    // Make sure the image is either 24 or 32 bpp
    const int bytesPerPixel = workingSurface->format->BytesPerPixel;
    if (bytesPerPixel < 3)
    {
        std::ostringstream ss;
        ss << "intermediate SDL_Surface for texture \"" << getName()
           << "\" is not truecolor (24 or 32 bpp)";
        throw SDLException(ss.str());
    }

    // Loop through the surface pixel data and override the alpha value for any pixels
    // that match the color key, if one was set
    uint8_t *originalPixels = static_cast<uint8_t *>(workingSurface->pixels);
    uint8_t *keyedPixels = new uint8_t[_width * _height * 4];
    assert(keyedPixels != nullptr);
    int i, j;
    for (i = 0, j = 0;
         i < _width * _height * bytesPerPixel;
         i += bytesPerPixel, j += 4)
    {
        if (colorKey != nullptr &&
            originalPixels[i]     == colorKey->r &&
            originalPixels[i + 1] == colorKey->g &&
            originalPixels[i + 2] == colorKey->b)
        {
            // If the color key matches, set the alpha byte to zero
            keyedPixels[j + 3] = 0;
        }
        else
        {
            keyedPixels[j + 3] = 255;
        }

        keyedPixels[j]     = originalPixels[i];
        keyedPixels[j + 1] = originalPixels[i + 1];
        keyedPixels[j + 2] = originalPixels[i + 2];
    }

    // Free the now-unnecessary SDL_Surface if it was optimized
    if (optimize)
        SDL_FreeSurface(workingSurface);

    // Prepare to create an OpenGL texture
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Copy the texture into video memory
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 _width,
                 _height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 keyedPixels);

    // Free the original pixel data
    delete[] keyedPixels;

    glGenerateMipmap(GL_TEXTURE_2D);
}
