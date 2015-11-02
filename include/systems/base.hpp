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

#ifndef __SYSTEMS_BASE_HPP__
#define __SYSTEMS_BASE_HPP__

#include "defines.hpp"

#include <list>
#include <memory>

#include "events/subscriber.hpp"
#include "stringable.hpp"

namespace Systems
{
    class Base : public Events::Subscriber, public Stringable
    {
    public:
        template <class T>
        using ComponentList = std::list<std::shared_ptr<T>>;

        Base();
        ~Base();

        virtual void destroy() = 0;

        std::string toString() const;

    protected:
        bool _destroyed;
    };
}

#endif
