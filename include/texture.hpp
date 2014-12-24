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
#include <SDL2/SDL.h>
#include <string>
#include "resourcemanager.hpp"

class Texture
{
public:
    Texture(const std::string &filename);
    Texture(SDL_Surface *surface, bool freeSurface = true, bool optimize = true);
    ~Texture();

private:
    // Make this private for now, since there's no functionality to release
    // an existing texture without a resource leak
    void load(const std::string &filename);

public:
    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }

    inline GLuint getRawTexture() const { return _texture; }

    std::string toString() const;

private:
    GLuint _texture;
    int _width, _height;

    static GLuint copySurfaceToGL(SDL_Surface *surface,
                                  bool optimize = true,
                                  int *outWidth = nullptr,
                                  int *outHeight = nullptr);
};

typedef ResourceManager<Texture> TextureManager;

#endif
