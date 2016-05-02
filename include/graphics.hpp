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

#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <string>
#include <unordered_map>

#include "color.hpp"
#include "exceptions.hpp"
#include "texture.hpp"
#include "vector.hpp"

// Miscellaneous graphics routines
namespace Graphics
{
    // Container class for drawing texture/sprite batches
    // TODO: reorganize this later
    class Batch
    {
    public:
        void draw();

        void queueBlit(TextureManager::Key &texture, const Vector2f &pos);
        void queueBlit(TextureManager::Key &texture, const Vector2f &&pos);

        inline void queueBlit(TextureManager::Key &texture, float x, float y)
        {
            queueBlit(texture, std::move(Vector2f(x, y)));
        }

        inline void queueBlit(TextureManager::Key &texture, const Vector2i &pos)
        {
            queueBlit(texture, std::move(Vector2f(pos.x, pos.y)));
        }

    private:
        typedef std::list<Vector2f> PositionList;
        typedef std::unordered_map<TextureManager::Key, PositionList> BatchMap;

        BatchMap _map;
    };

    // NOTE: The default arguments for "optimize" and "freeSurface" were
    //       originally true, but have been changed to false because
    //       SDL_FreeSurface kept erroring out. Investigate this later!

    // SDL_Surface creation/manipulation
    SDL_Surface *loadSDLSurface(const std::string &filename,
                                bool optimize = false);
    SDL_Surface *optimizeSDLSurface(SDL_Surface *surface,
                                    bool freeSurface = false);
    SDL_Surface *createBlankSDLSurface(int width,
                                       int height,
                                       SDL_Color color = Color::WHITE);
    inline SDL_Surface *createBlankSDLSurface(Vector2i dimensions,
                                              SDL_Color color = Color::WHITE)
    {
        return createBlankSDLSurface(dimensions.x, dimensions.y, color);
    }

    // Screen blitting
    void blitTexture(const TextureManager::Key &texture,
                     float x,
                     float y);

    inline void blitTexture(const TextureManager::Key &texture,
                     const Vector2f &pos)
    {
        blitTexture(texture, pos.x, pos.y);
    }

    inline void blitTexture(const TextureManager::Key &texture,
                     const Vector2i &pos)
    {
        blitTexture(texture, pos.x, pos.y);
    }
}

namespace Exceptions
{
    class SDLException : public Base
    {
    public:
        SDLException(const std::string &msg) : Base(msg) {}
        SDLException() : Base(SDL_GetError()) {}
    };

    class SDLImageException : public SDLException
    {
    public:
        SDLImageException(const std::string &msg) : SDLException(msg) {}
        SDLImageException() : SDLException(IMG_GetError()) {}
    };

    class GLException : public Base
    {
    public:
        GLException(const std::string &msg) : Base(msg) {}
        explicit GLException(GLenum error) : Base(getErrorString(error)) {}

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
}

#endif
