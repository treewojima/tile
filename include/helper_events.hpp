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

#ifndef __HELPER_EVENTS_HPP__
#define __HELPER_EVENTS_HPP__

#include "defines.hpp"
#include "game.hpp"

namespace Game
{
    // Helper key handler event for registerEvent()
    class KeyEvent : public Event
    {
    public:
        KeyEvent(SDL_Scancode key,
                 Event::Callback callback,
                 const std::string debugString_) :
            Event(debugString_),
            _key(key),
            _callback(callback) {}

        bool test(const SDL_Event &e);
        void fire(const SDL_Event &e);

    private:
        SDL_Scancode _key;
        Game::Event::Callback _callback;
    };

    // Program quit handling event, for signals such as ^C
    class QuitEvent : public Event
    {
    public:
        QuitEvent() : Event("QuitEvent") {}

        bool test(const SDL_Event &e);
        void fire(const SDL_Event &e);
    };
}

#endif
