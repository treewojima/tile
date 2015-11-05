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
#include "graphics.hpp"

#include <cassert>
#include <SDL2/SDL_image.h>

#include "colors.hpp"
#include "exception.hpp"
#include "game.hpp"

SDL_Surface *Graphics::loadSDLSurface(const std::string &filename,
                                      bool optimize)
{
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::ostringstream ss;
        ss << "unable to load \"" << filename << "\": " << IMG_GetError();
        throw SDLImageException(ss.str());
    }

    if (optimize)
    {
        surface = optimizeSDLSurface(surface);
    }

    return surface;
}

#if 0
SDL_Surface *Graphics::loadSDLSurface(const std::string &filename,
                                      SDL_Color colorKey,
                                      bool optimize)
{
    auto surface = loadSDLSurface(filename, optimize);
    SDL_SetColorKey(surface,
                    SDL_TRUE,
                    Colors::convertToUint32(colorKey, surface->format));
    return surface;
}
#endif

SDL_Surface *Graphics::optimizeSDLSurface(SDL_Surface *surface, bool freeSurface)
{
    assert(surface != nullptr);

    auto optimizedSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                 surface->w,
                                                 surface->h,
                                                 32,
                                                 Colors::RMASK,
                                                 Colors::GMASK,
                                                 Colors::BMASK,
                                                 Colors::AMASK);
    if (!optimizedSurface)
        throw SDLException();

    SDL_BlitSurface(surface, nullptr, optimizedSurface, nullptr);

    if (freeSurface)
        SDL_FreeSurface(surface);

    return optimizedSurface;
}

SDL_Surface *Graphics::createBlankSDLSurface(int width, int height, SDL_Color color)
{
    auto surface = SDL_CreateRGBSurface(0,
                                        width,
                                        height,
                                        32,
                                        Colors::RMASK,
                                        Colors::GMASK,
                                        Colors::BMASK,
                                        Colors::AMASK);
    if (surface == nullptr)
        throw SDLException();

    SDL_FillRect(surface,
                 nullptr,
                 SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));

    return surface;
}

void Graphics::blitTexture(const TextureManager::Key &texture,
                           float x,
                           float y)
{
    const auto &texturePtr = Game::getTexMgr().get(texture);

    const auto width = texturePtr->getWidth();
    const auto height = texturePtr->getHeight();
    const float centeredWidth = width / 2.f;
    const float centeredHeight = height / 2.f;

    // NOTE: Should there be a glLoadIdentity() here as well?

    glBindTexture(GL_TEXTURE_2D, texturePtr->getRawTexture());
    glTranslatef(x - centeredWidth, y - centeredHeight, 0);
    glBegin(GL_QUADS);
        glTexCoord2i(1, 1);
        glVertex2f(width, 0);
        glTexCoord2i(0, 1);
        glVertex2f(0, 0);
        glTexCoord2i(0, 0);
        glVertex2f(0, height);
        glTexCoord2i(1, 0);
        glVertex2f(width, height);
    glEnd();

    glLoadIdentity();
}
