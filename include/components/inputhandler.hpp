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

#ifndef __INPUTHANDLER_HPP__
#define __INPUTHANDLER_HPP__

#include "defines.hpp"

#include <functional>

#include "components/base.hpp"

class Entity;

namespace Components
{
    class InputHandler : public Base
    {
    public:
        typedef std::function<void(uint8_t *)> Callback;

    private:
        InputHandler(std::shared_ptr<Entity> parent, const Callback &callback_);
        InputHandler(std::shared_ptr<Entity> parent, const Callback &&callback_);

    public:
        Callback callback;

        std::string toString() const;

        static std::shared_ptr<InputHandler> create(std::shared_ptr<Entity> parent,
                                                    const Callback &callback_);
        static std::shared_ptr<InputHandler> create(std::shared_ptr<Entity> parent,
                                                    const Callback &&callback_);
    };
}

#endif

