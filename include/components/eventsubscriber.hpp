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

#ifndef __COMPONENTS_EVENTSUBSCRIBER_HPP__
#define __COMPONENTS_EVENTSUBSCRIBER_HPP__

#include "defines.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"

namespace Components
{
    template <class T>
    class BaseEventSubscriber : public Base
    {
    public:
        static std::shared_ptr<BaseEventSubscriber<T>>
            create(const Entity::UUID &parent,
                   std::shared_ptr<T> subscriber_,
                   const std::string &debugName = "")
        {
            auto ptr = std::shared_ptr<BaseEventSubscriber<T>>(
                        new BaseEventSubscriber(parent, subscriber_, debugName));
            Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
            Events::Dispatcher::raise<
                    Events::SpecificComponentCreated<BaseEventSubscriber<T>>>(ptr);
            return ptr;
        }

    private:
        BaseEventSubscriber(const Entity::UUID &parent,
                            std::shared_ptr<T> subscriber_,
                            const std::string &debugName) :
            Base(parent),
            subscriber(subscriber_)
        {
            static_assert(std::is_base_of<T, Events::Subscriber>::value ||
                          std::is_base_of<T, Events::AsyncSubscriber>::value,
                          "must be initialized from type Events::(Async)Subscriber");

            if (!debugName.length())
            {
                setDebugName(getParent()->getDebugName() +
                             boost::core::demangle(typeid(T).name()));
            }
        }

    public:
        std::shared_ptr<T> subscriber;

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Components::";

            if (std::is_same<T, Events::AsyncSubscriber>::value)
                ss << "Async";

            ss << "EventSubscriber[debugName = \"" << getDebugName() << "\", "
               << "parent = " << getParent() << "\", "
               << "subscriber = " << subscriber << "]";

            return ss.str();
        }
    };

    typedef BaseEventSubscriber<Events::Subscriber> EventSubscriber;
    typedef BaseEventSubscriber<Events::AsyncSubscriber> AsyncEventSubscriber;
}

namespace Events
{
    typedef SpecificComponentCreated<Components::EventSubscriber>
            EventSubscriberComponentCreated;

    typedef SpecificComponentCreated<Components::AsyncEventSubscriber>
            AsyncEventSubscriberComponentCreated;
}

#endif
