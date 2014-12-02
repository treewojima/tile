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
#include "helper_events.hpp"

#include <easylogging++.h>
#include <iostream>

bool Game::KeyEvent::test(const SDL_Event &e)
{
    //return (e.type == SDL_KEYDOWN &&
    //        e.key.keysym.sym == _key);
    return (e.type == CUSTOM_KEYPRESS_EVENT &&
            static_cast<const Uint8 *>(e.user.data1)[_key]);
}

void Game::KeyEvent::fire(const SDL_Event &e)
{
    _callback(e);
}

bool Game::QuitEvent::test(const SDL_Event &e)
{
    return (e.type == SDL_QUIT);
}

void Game::QuitEvent::fire(const SDL_Event &e)
{
    Game::setRunning(false);
    LOG(INFO) << "received quit signal";
}
