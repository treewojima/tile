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

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "defines.hpp"

#include <GL/glew.h>
#include <memory>
#include <SDL2/SDL.h>
#include <string>

#include "resourcemanager.hpp"
#include "stringable.hpp"

#ifdef _DEBUG
//#   define _TEST_TEXTURE_DIMENSIONS
//#   define _DEBUG_TEXTURES
#endif

class Texture : public Stringable
{
public:
    Texture(const std::string &name,
            const std::string &filename,
            const SDL_Color *colorKey = nullptr);
    Texture(const std::string &name,
            SDL_Surface *surface,
            const SDL_Color *colorKey = nullptr,
#ifdef PRESERVE_TEXTURE_SURFACE
            bool freeSurface = false,
#else
            bool freeSurface = true,
#endif
            bool optimize = true);
    ~Texture();

    inline std::string getName() const { return _name; }
    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }
    inline GLuint getRawTexture() const { return _texture; }
#ifdef PRESERVE_TEXTURE_SURFACE
    inline SDL_Surface *getSurface() const { return _surface; }
#endif

    std::string toString() const;

private:
    std::string _name;
    GLuint _texture;
    int _width, _height;
#ifdef PRESERVE_TEXTURE_SURFACE
    SDL_Surface *_surface;
#endif

    void copySurfaceToGL(SDL_Surface *surface,
                         const SDL_Color *colorKey = nullptr,
                         bool optimize = true);
};

typedef ResourceManager<std::shared_ptr<Texture>> TextureManager;

#endif
