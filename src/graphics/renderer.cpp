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

#include "graphics/renderer.hpp"

#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.hpp"

Graphics::Renderer::Renderer()
{
    // Initialize SDL graphics subsystem
    if (SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    // Initialize SDL_Image
    auto flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(flags) & flags))
    {
        throw Exceptions::RendererException(IMG_GetError());
    }

    // Create window and renderer
    _window = std::make_shared<Window>();

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (getGame().getOptions().vsync)
    {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    _renderer = SDL_CreateRenderer(_window->_window, -1, flags);
    if (!_renderer)
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    // Set clear color
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
}

Graphics::Renderer::~Renderer()
{
    destroy();
}

void Graphics::Renderer::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    SDL_DestroyRenderer(_renderer);
    _window->destroy();
    _window.reset();

    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    destroyed = true;
}

void Graphics::Renderer::clear()
{
    assert(_renderer);
    SDL_RenderClear(_renderer);
}

void Graphics::Renderer::present()
{
    assert(_renderer);
    SDL_RenderPresent(_renderer);
}

void Graphics::Renderer::blitToScreen(const TextureManager::Key &texture,
                                      const Vector2i &pos)
{
    blitToScreen(texture, pos.x, pos.y);
}

void Graphics::Renderer::blitToScreen(const TextureManager::Key &texture_,
                                      int x,
                                      int y)
{
    std::shared_ptr<Texture> texture = getGame().getTexMgr()[texture_];

    auto dimensions = texture->getDimensions();
    SDL_Rect r = { x + dimensions.x / 2,
                   y + dimensions.y / 2,
                   dimensions.x,
                   dimensions.y };

    blit(texture->_texture, nullptr, &r);
}

void Graphics::Renderer::blitToTexture(const TextureManager::Key &src_,
                                       SDL_Rect *srcRect,
                                       const TextureManager::Key &dest_,
                                       SDL_Rect *destRect)
{
    std::shared_ptr<Texture> src = getGame().getTexMgr()[src_];
    std::shared_ptr<Texture> dest = getGame().getTexMgr()[dest_];

    if (SDL_SetRenderTarget(_renderer, dest->_texture))
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    blit(src->_texture, srcRect, destRect);

    SDL_SetRenderTarget(_renderer, nullptr);
}

void Graphics::Renderer::blit(SDL_Texture *texture,
                              SDL_Rect *srcRect,
                              SDL_Rect *destRect)
{
    assert(_renderer);
    assert(texture);
    SDL_RenderCopy(_renderer, texture, srcRect, destRect);
}


