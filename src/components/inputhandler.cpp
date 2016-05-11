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

#include "components/inputhandler.hpp"

#include <sstream>

#include "entity.hpp"
#include "events/dispatcher.hpp"

std::shared_ptr<Components::InputHandler>
Components::InputHandler::create(const Entity::UUID &parent,
                                 const Callback &&callback_,
                                 const std::string &debugName)
{
    auto ptr = std::shared_ptr<InputHandler>(
                new InputHandler(parent,
                                 std::move(callback_),
                                 debugName));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::InputHandlerComponentCreated>(ptr);
    return ptr;
}

Components::InputHandler::InputHandler(const Entity::UUID &parent,
                                       const Callback &&callback_,
                                       const std::string &debugName) :
    Base(parent, debugName),
    callback(std::move(callback_))
{
}

std::string Components::InputHandler::toString() const
{
    std::ostringstream ss;
    ss << "Components::InputHandler[callback = "
       << "<FIXME: can't access std::function in const method>]";
    return ss.str();
}
