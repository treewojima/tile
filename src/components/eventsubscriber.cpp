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

#include "components/eventsubscriber.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"

Components::EventSubscriber::EventSubscriber(
        std::shared_ptr<Entity> parent,
        std::shared_ptr<Events::Subscriber> _subscriber) :
    Base(parent, parent->getDebugName() + "EventSubscriber"),
    subscriber(_subscriber)
{
}

std::string Components::EventSubscriber::toString() const
{
    std::ostringstream ss;
    ss << "Components::EventSubscriber[debugName = \"" << getDebugName() << "\", "
       << "parent = " << getParent() << "\", "
       << "subscriber = " << subscriber << "]";
    return ss.str();
}

std::shared_ptr<Components::EventSubscriber>
Components::EventSubscriber::create(
        std::shared_ptr<Entity> parent,
        std::shared_ptr<Events::Subscriber> _subscriber)
{
    auto ptr = std::shared_ptr<EventSubscriber>(
                new EventSubscriber(parent, _subscriber));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::EventSubscriberComponentCreated>(ptr);
    return ptr;
}

Components::AsyncEventSubscriber::AsyncEventSubscriber(
        std::shared_ptr<Entity> parent,
        std::shared_ptr<Events::AsyncSubscriber> _subscriber) :
    Base(parent, parent->getDebugName() + "AsyncEventSubscriber"),
    subscriber(_subscriber)
{
}

std::string Components::AsyncEventSubscriber::toString() const
{
    std::ostringstream ss;
    ss << "Components::AsyncEventSubscriber[debugName = \"" << getDebugName() << "\", "
       << "parent = " << getParent() << "\", "
       << "subscriber = " << subscriber << "]";
    return ss.str();
}

std::shared_ptr<Components::AsyncEventSubscriber>
Components::AsyncEventSubscriber::create(
        std::shared_ptr<Entity> parent,
        std::shared_ptr<Events::AsyncSubscriber> _subscriber)
{
    auto ptr = std::shared_ptr<AsyncEventSubscriber>(
                new AsyncEventSubscriber(parent, _subscriber));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::AsyncEventSubscriberComponentCreated>(ptr);
    return ptr;
}
