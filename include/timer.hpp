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

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "defines.hpp"

class Timer
{
public:
    enum class State
    {
        STOPPED,
        RUNNING,
        PAUSED
    };

    Timer();
    ~Timer();

    void start();
    void stop();
    void pause();
    void resume();

    long getTicks() const;
    inline State getState() const { return _state; }

private:
    long _startTicks;
    long _pausedTicks;
    State _state;
};

#endif
