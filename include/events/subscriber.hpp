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

#ifndef __EVENTS_SUBSCRIBER_HPP__
#define __EVENTS_SUBSCRIBER_HPP__

#include "defines.hpp"

namespace Events
{
    // For use with static_assert
    //class SubscriberBase : public Stringable {};

    // Must guarantee the following function:
    // (virtual) void onEvent(const <subclass of Event> &event)
    class Subscriber {};

    // Must guarantee the following function:
    // (virtual) void queueEvent(std::shared_ptr<subclass of Event> event)
    class AsyncSubscriber {};
}

#endif
