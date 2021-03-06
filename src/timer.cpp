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

#include "defines.hpp"
#include "timer.hpp"

#include <SDL2/SDL.h>

#include "logger.hpp"

bool Timer::_initialized = false;

Timer::Timer() :
    _startTicks(0),
    _pausedTicks(0),
    _state(State::STOPPED)
{
    if (!_initialized)
    {
        SDL_InitSubSystem(SDL_INIT_TIMER);
        _initialized = true;
    }
}

void Timer::start()
{
    _state = State::RUNNING;
    _startTicks = SDL_GetTicks();
    _pausedTicks = 0;

    //BOOST_LOG_TRIVIAL(debug) << "starting timer";
}

void Timer::stop()
{
    //BOOST_LOG_TRIVIAL(debug) << "stopped timer after " << getTicks() << " ticks";

    _state = State::STOPPED;
    _startTicks = _pausedTicks = 0;
}

void Timer::pause()
{
    if (_state != State::RUNNING)
    {
        LOG_WARNING << "tried to pause non-running timer";
    }
    else
    {
        _state = State::PAUSED;
        _pausedTicks = SDL_GetTicks() - _startTicks;
        _startTicks = 0;

        //BOOST_LOG_TRIVIAL(debug) << "pausing timer at " << getTicks() << " ticks";
    }
}

void Timer::resume()
{
    if (_state != State::PAUSED)
    {
        LOG_WARNING << "tried to resume non-paused timer";
    }
    else
    {
        _state = State::RUNNING;
        _startTicks = SDL_GetTicks() - _pausedTicks;
        _pausedTicks = 0;

        //BOOST_LOG_TRIVIAL(debug) << "resuming timer at " << getTicks() << " ticks";
    }
}

long Timer::getTicks() const
{
    long time = 0;

    switch (_state)
    {
    case State::STOPPED:
        break;

    case State::RUNNING:
        time = SDL_GetTicks() - _startTicks;
        break;

    case State::PAUSED:
        time = _pausedTicks;
    }

    return time;
}
