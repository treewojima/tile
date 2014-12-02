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

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "defines.hpp"
#include <functional>
#include <SDL2/SDL.h>
#include <string>

#include "camera.hpp"

namespace Game
{
    struct Options
    {
        int windowWidth, windowHeight;
    };

    class Event
    {        
    public:
        typedef int Handle;
        typedef std::function<void (const SDL_Event &e)> Callback;

        Event(const std::string &debugString_ = std::string()) :
            debugString(debugString_) {}

        virtual ~Event()
        {
#ifdef _DEBUG_EVENTS
            LOG(DEBUG) << "destroyed event \"" << debugString << "\"";
#endif
        }

        virtual bool test(const SDL_Event &e) = 0;
        virtual void fire(const SDL_Event &e) = 0;

        std::string debugString;

        // This doesn't quite work as const since the value can't be determined
        // at runtime, so... don't fuck with this!
        static Uint32 CUSTOM_KEYPRESS_EVENT;
    };

    void run(Options options);

    bool isRunning();
    void setRunning(bool b);

    Camera &getCamera();

    Event::Handle registerEvent(SDL_Scancode key,
                                Event::Callback callback,
                                const std::string &debugString = std::string());
    Event::Handle registerEvent(std::shared_ptr<Event> event);
    void unregisterEvent(Event::Handle handle);
}

#endif
