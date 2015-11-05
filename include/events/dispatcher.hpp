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

#ifndef __EVENTS_DISPATCHER_HPP__
#define __EVENTS_DISPATCHER_HPP__

#include "defines.hpp"

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <utility>

#ifdef _DEBUG_DISPATCHER_MAP
#   warning "using debug unordered_map"
#   include <debug/unordered_map>
#   define DispatcherMap __gnu_debug::unordered_map
#else
#   include <unordered_map>
#   define DispatcherMap std::unordered_map
#endif

#include "components/base.hpp"
#include "events/base.hpp"
#include "events/subscriber.hpp"

#ifdef _DEBUG_NEW_EVENTS
#   include <boost/core/demangle.hpp>
#   include "logger.hpp"
#endif

namespace Events
{
    class Dispatcher
    {
    private:
        Dispatcher() {}

    public:
        template <class E, class T>
        static void subscribe(T &subscriber);

        template <class E, class T>
        static void unsubscribe(T &subscriber);

        template <class T>
        static void unsubscribe(T &subscriber);

        template <class E, class... Args>
        static void raise(Args&& ... args);

    private:
        typedef std::pair<Subscriber *, std::function<void(const Base &)>> SubscriberCallbackPair;
        typedef std::list<SubscriberCallbackPair> SubscriberList;
        typedef DispatcherMap<std::type_index, SubscriberList> TypeSubscriberMap;

        static TypeSubscriberMap _map;
    };
}

template <class E, class T>
void Events::Dispatcher::subscribe(T &subscriber)
{
    static_assert(std::is_base_of<Base, E>::value,
                  "Can only subscribe to types derived from class Event");
    static_assert(std::is_base_of<Subscriber, T>::value,
                  "Only classes derived from Subscriber can be subscribers");

    auto callback = [&subscriber](const Events::Base &event)
        { subscriber.onEvent(static_cast<const E &>(event)); };

    _map[typeid(E)].push_back(
                std::make_pair(static_cast<Subscriber *>(&subscriber),
                               callback));

#ifdef _DEBUG_NEW_EVENTS
	LOG_DEBUG << "subscriber " << subscriber
              << " is listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class E, class T>
void Events::Dispatcher::unsubscribe(T &subscriber)
{
    // Are these checks necessary?
    static_assert(std::is_base_of<Base, E>::value,
                  "Can only unsubscribe from types derived from class Event");
    static_assert(std::is_base_of<Subscriber, T>::value,
                  "Only classes derived from Subscriber can be subscribers");

    auto s = static_cast<Subscriber *>(&subscriber);
    auto i = _map.find(typeid(E));
    if (i != _map.end())
    {
        auto &list = i->second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_NEW_EVENTS
	LOG_DEBUG << "subscriber " << subscriber
              << " is no longer listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class T>
void Events::Dispatcher::unsubscribe(T &subscriber)
{
    static_assert(std::is_base_of<Subscriber, T>::value,
                  "Only classes derived from Subscriber can be subscribers");

    auto s = static_cast<Subscriber *>(&subscriber);

    for (auto &i : _map)
    {
        auto &list = i.second;
        for (auto j = list.begin(); j != list.end(); j++)
        {
            if (j->first == s)
            {
                list.erase(j);
                break;
            }
        }
    }

#ifdef _DEBUG_NEW_EVENTS
	LOG_DEBUG << "subscriber " << subscriber
			  << " is no longer listening for events of any type";
#endif
}

template <class E, class... Args>
void Events::Dispatcher::raise(Args&& ... args)
{
    static_assert(std::is_base_of<Base, E>::value,
                  "Can only raise types derived from class Event");

    E event(std::forward<Args>(args)...);

    auto list = _map[typeid(E)];
    for (auto &pair : list)
    {
        //callback(static_cast<const Base &>(event));
        pair.second(static_cast<const Base &>(event));
    }

#ifdef _DEBUG_NEW_EVENTS
	LOG_DEBUG << "event raised: " << event;
#endif
}

#endif
