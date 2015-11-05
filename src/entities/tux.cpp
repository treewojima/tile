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

#ifndef _USE_NEW_ENTITY

#include "entities/tux.hpp"

Tux::Tux() :
    Entity("tux"),
    _eventHandles(4)
{
    // NOTE: This should all be refactored into some sort of component

    static const float magnitude = 5;

    _eventHandles.push_back(Game::registerEvent(
        SDL_SCANCODE_UP,
        [this](const SDL_Event &e) { position->y += magnitude; },
        "TuxUpEvent"));

    _eventHandles.push_back(Game::registerEvent(
        SDL_SCANCODE_DOWN,
        [this](const SDL_Event &e) { position->y -= magnitude; },
        "TuxDownEvent"));

    _eventHandles.push_back(Game::registerEvent(
        SDL_SCANCODE_LEFT,
        [this](const SDL_Event &e) { position->x -= magnitude; },
        "TuxLeftEvent"));

    _eventHandles.push_back(Game::registerEvent(
        SDL_SCANCODE_RIGHT,
        [this](const SDL_Event &e) { position->x += magnitude; },
        "TuxRightEvent"));
}

Tux::~Tux()
{
    for (auto handle : _eventHandles)
    {
        Game::unregisterEvent(handle);
    }
}

#endif
