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

#ifndef __EVENTS_DISPATCHER_HPP__
#define __EVENTS_DISPATCHER_HPP__

#include "defines.hpp"

#include <boost/core/demangle.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include "events/base.hpp"
#include "events/subscriber.hpp"
#ifdef _DEBUG_NEW_EVENTS
#   include "logger.hpp"
#endif

namespace Events
{
    // TODO: This class is ripe for refactoring
    class Dispatcher
    {
    private:
        Dispatcher() {}

    public:
        template <class E,
                  class T,
                  typename std::enable_if<
                      std::is_base_of<Events::Subscriber, T>::value>::type* = nullptr>
        static void subscribe(T &subscriber)
        {
            static_assert(std::is_base_of<Base, E>::value,
                          "Can only subscribe to types derived from class Events::Base");
            static_assert(std::is_base_of<Subscriber, T>::value,
                          "Only subclasses of class Subscriber can subscribe to asynchronous events");

            subscribeSync<E>(subscriber);
        }

        template <class E,
                  class T,
                  typename std::enable_if<
                      std::is_base_of<Events::AsyncSubscriber, T>::value>::type* = nullptr>
        static void subscribe(T &subscriber)
        {
            static_assert(std::is_base_of<Base, E>::value,
                          "Can only subscribe to types derived from class Events::Base");

            subscribeAsync<E>(subscriber);
        }

        template <class E,
                  class T,
                  typename std::enable_if<
                      std::is_base_of<Events::Subscriber, T>::value>::type* = nullptr>
        static void unsubscribe(T &subscriber)
        {
            static_assert(std::is_base_of<Base, E>::value,
                          "Can only unsubscribe from types derived from class Events::Base");

            unsubscribeSync<E>(subscriber);
        }

        template <class E,
                  class T,
                  typename std::enable_if<
                      std::is_base_of<Events::AsyncSubscriber, T>::value>::type* = nullptr>
        static void unsubscribe(T &subscriber)
        {
            static_assert(std::is_base_of<Base, E>::value,
                          "Can only unsubscribe from types derived from class Events::Base");

            unsubscribeAsync<E>(subscriber);
        }

        template <class T,
                  typename std::enable_if<
                      std::is_base_of<Events::Subscriber, T>::value>::type* = nullptr>
        static void unsubscribe(T &subscriber)
        {
            unsubscribeSync(subscriber);
        }

        template <class T,
                  typename std::enable_if<
                      std::is_base_of<Events::AsyncSubscriber, T>::value>::type* = nullptr>
        static void unsubscribe(T &subscriber)
        {
            unsubscribeAsync(subscriber);
        }

        template <class E, class... Args>
        static void raise(Args&& ... args);

    private:
        template <class E, class T>
        static void subscribeSync(T &subscriber);

        template <class E, class T>
        static void subscribeAsync(T &subscriber);

        template <class E, class T>
        static void unsubscribeSync(T &subscriber);

        template <class E, class T>
        static void unsubscribeAsync(T &subscriber);

        template <class T>
        static void unsubscribeSync(T &subscriber);

        template <class T>
        static void unsubscribeAsync(T &subscriber);

        typedef std::function<void(const Base &)> SubscriberCallback;
        typedef std::pair<Subscriber *, SubscriberCallback> SubscriberPair;
        typedef std::list<SubscriberPair> SubscriberList;
        typedef std::unordered_map<std::type_index, SubscriberList> TypeSubscriberMap;

        typedef std::function<void(std::shared_ptr<Base>)> AsyncSubscriberCallback;
        typedef std::pair<AsyncSubscriber *, AsyncSubscriberCallback> AsyncSubscriberPair;
        typedef std::list<AsyncSubscriberPair> AsyncSubscriberList;
        typedef std::unordered_map<std::type_index, AsyncSubscriberList> TypeAsyncSubscriberMap;

        static TypeSubscriberMap _map;
        static TypeAsyncSubscriberMap _asyncMap;
    };
}

template <class E, class... Args>
void Events::Dispatcher::raise(Args&& ... args)
{
    static_assert(std::is_base_of<Base, E>::value,
                  "Can only raise types derived from class Events::Base");

    auto event = std::make_shared<E>(std::forward<Args>(args)...);
    auto baseEvent = std::static_pointer_cast<Base>(event);

#ifdef _DEBUG_NEW_EVENTS
    LOG_DEBUG << "event raised: " << event;
#endif

    // First, queue it for asynchronous subscribers
    auto asyncList = _asyncMap[typeid(E)];
    for (auto &pair : asyncList)
    {
        pair.second(baseEvent);
    }

    // Next, fire off regular subscribers
    auto list = _map[typeid(E)];
    for (auto &pair : list)
    {
        pair.second(*baseEvent);
    }
}

template <class E, class T>
void Events::Dispatcher::subscribeSync(T &subscriber)
{
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
void Events::Dispatcher::subscribeAsync(T &subscriber)
{
    auto callback = [&subscriber](std::shared_ptr<Events::Base> event)
        { subscriber.queueEvent(std::static_pointer_cast<E>(event)); };

    _asyncMap[typeid(E)].push_back(
                std::make_pair(static_cast<AsyncSubscriber *>(&subscriber),
                               callback));

#ifdef _DEBUG_NEW_EVENTS
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class E, class T>
void Events::Dispatcher::unsubscribeSync(T &subscriber)
{
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

template <class E, class T>
void Events::Dispatcher::unsubscribeAsync(T &subscriber)
{
    auto s = static_cast<AsyncSubscriber *>(&subscriber);
    auto i = _asyncMap.find(typeid(E));
    if (i != _asyncMap.end())
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
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is no longer listening for events of type "
              << boost::core::demangle(typeid(E).name());
#endif
}

template <class T>
void Events::Dispatcher::unsubscribeSync(T &subscriber)
{
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

template <class T>
void Events::Dispatcher::unsubscribeAsync(T &subscriber)
{
    auto s = static_cast<AsyncSubscriber *>(&subscriber);

    for (auto &i : _asyncMap)
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
    LOG_DEBUG << "asynchronous subscriber " << subscriber
              << " is no longer listening for events of any type";
#endif
}

#endif
