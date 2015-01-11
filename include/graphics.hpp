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

#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include "defines.hpp"

#include <SDL2/SDL.h>
#include <string>

#include "colors.hpp"
#include "texture.hpp"
#include "vector.hpp"

// Miscellaneous graphics routines
namespace Graphics
{
    // NOTE: The default arguments for "optimize" and "freeSurface" were
    //       originally true, but have been changed to false because
    //       SDL_FreeSurface kept erroring out. Investigate this later!

    // SDL_Surface creation/manipulation
    SDL_Surface *loadSDLSurface(const std::string &filename,
                                bool optimize = false);
    //SDL_Surface *loadSDLSurface(const std::string &filename,
    //                            SDL_Color colorKey,
    //                            bool optimize = true);
    SDL_Surface *optimizeSDLSurface(SDL_Surface *surface,
                                    bool freeSurface = false);
    SDL_Surface *createBlankSDLSurface(int width,
                                       int height,
                                       SDL_Color color = Colors::WHITE);
    inline SDL_Surface *createBlankSDLSurface(Vector2i dimensions,
                                              SDL_Color color = Colors::WHITE)
    {
        return createBlankSDLSurface(dimensions.x, dimensions.y, color);
    }

    // Screen blitting
    void blitTexture(TextureManager::ConstResourcePtr texture,
                     float x,
                     float y);
    inline void blitTexture(TextureManager::ConstResourcePtr texture,
                     const Vector2f &pos)
    {
        blitTexture(texture, pos.x, pos.y);
    }

    inline void blitTexture(TextureManager::ConstResourcePtr texture,
                     const Vector2i &pos)
    {
        blitTexture(texture, pos.x, pos.y);
    }
}

#endif
