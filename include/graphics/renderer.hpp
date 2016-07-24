/* game
 * Copyright (C) 2014-2016 Scott Bishop <treewojima@gmail.com>
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

#ifndef __GRAPHICS_RENDERER_HPP__
#define __GRAPHICS_RENDERER_HPP__

#include "defines.hpp"

#include <SDL2/SDL.h>

#include "exceptions.hpp"
#include "graphics/texture.hpp"
#include "graphics/window.hpp"

namespace Graphics
{
    class Renderer
    {
        friend class Texture;

    public:
        Renderer();
        ~Renderer();

        void destroy();

        void clear();
        void present();

        void blitToScreen(const TextureManager::Key &texture, const Vector2i &pos);
        void blitToScreen(const TextureManager::Key &texture_, int x, int y);

        void blitToTexture(const TextureManager::Key &src_,
                           SDL_Rect *srcRect,
                           const TextureManager::Key &dest_,
                           SDL_Rect *destRect);

        inline Window *getWindow() { return _window; }

    private:
        Window *_window;
        SDL_Renderer *_renderer;

        void blit(SDL_Texture *texture,
                  SDL_Rect *srcRect,
                  SDL_Rect *destRect);
    };
}

namespace Exceptions
{
    class RendererException : public GraphicsException
    {
    public:
        using GraphicsException::GraphicsException;
    };
}

#endif
