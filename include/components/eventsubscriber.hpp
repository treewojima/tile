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

#ifndef __COMPONENTS_EVENTSUBSCRIBER_HPP__
#define __COMPONENTS_EVENTSUBSCRIBER_HPP__

#include "defines.hpp"
#include "components/base.hpp"
#include "events/base.hpp"
#include "events/subscriber.hpp"

namespace Components
{
    class EventSubscriber : public Base
    {
    private:
        EventSubscriber(std::shared_ptr<Entity> parent,
                        std::shared_ptr<Events::Subscriber> _subscriber);

    public:
        std::shared_ptr<Events::Subscriber> subscriber;

        std::string toString() const;

        static std::shared_ptr<EventSubscriber>
            create(std::shared_ptr<Entity> parent,
                   std::shared_ptr<Events::Subscriber> _subscriber);
    };

    class AsyncEventSubscriber : public Base
    {
    private:
        AsyncEventSubscriber(std::shared_ptr<Entity> parent,
                             std::shared_ptr<Events::AsyncSubscriber> _subscriber);

    public:
        std::shared_ptr<Events::AsyncSubscriber> subscriber;

        std::string toString() const;

        static std::shared_ptr<AsyncEventSubscriber>
            create(std::shared_ptr<Entity> parent,
                   std::shared_ptr<Events::AsyncSubscriber> _subscriber);
    };
}

namespace Events
{
    typedef SpecificComponentCreated<Components::EventSubscriber>
            EventSubscriberComponentCreated;

    typedef SpecificComponentCreated<Components::AsyncEventSubscriber>
            AsyncEventSubscriberComponentCreated;
}

#endif
