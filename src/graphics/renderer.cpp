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
        throw Exceptions::RendererException();
    }

    // Initialize SDL_Image
    auto flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(flags) & flags))
    {
        throw Exceptions::RendererException();
    }

    // Create window and renderer
    _window = new Window();

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (getGame().getOptions().vsync)
    {
        flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    _renderer = SDL_CreateRenderer(_window->_window, -1, flags);
    if (!_renderer)
    {
        throw Exceptions::RendererException();
    }

    // Set clear color
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
}

Graphics::Renderer::~Renderer()
{
    assert(_renderer);
    SDL_DestroyRenderer(_renderer);
    delete _window;

    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
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
    auto *texture = getGame().getTexMgr()[texture_];

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
    auto *src = getGame().getTexMgr()[src_];
    auto *dest = getGame().getTexMgr()[dest_];

    if (SDL_SetRenderTarget(_renderer, dest->_texture))
    {
        throw Exceptions::RendererException(SDL_GetError());
    }

    blit(src->_texture, srcRect, destRect);

    SDL_SetRenderTarget(_renderer, nullptr);
}

void Graphics::Renderer::drawLine(const SDL_Color &color,
                                  const Vector2i &start,
                                  const Vector2i &end)
{
    drawLine(color, start.x, start.y, end.x, end.y);
}

void Graphics::Renderer::drawLine(const SDL_Color &color,
                                  int x1, int y1,
                                  int x2, int y2)
{
    // Preserve existing draw color
    SDL_Color prevColor;
    SDL_GetRenderDrawColor(_renderer,
                           &prevColor.r,
                           &prevColor.g,
                           &prevColor.b,
                           &prevColor.a);

    SDL_SetRenderDrawColor(_renderer,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
    SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);

    // Reset previous draw color
    SDL_SetRenderDrawColor(_renderer,
                           prevColor.r,
                           prevColor.g,
                           prevColor.b,
                           prevColor.a);
}

void Graphics::Renderer::blit(SDL_Texture *texture,
                              SDL_Rect *srcRect,
                              SDL_Rect *destRect)
{
    assert(_renderer);
    assert(texture);
    SDL_RenderCopy(_renderer, texture, srcRect, destRect);
}


