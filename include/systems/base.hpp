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

#ifndef __SYSTEMS_BASE_HPP__
#define __SYSTEMS_BASE_HPP__

#include "defines.hpp"

#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "stringable.hpp"

namespace Systems
{
    class Base : public Stringable
    {
    public:
        template <class T>
        using ComponentMap = std::unordered_map<std::type_index, T*>;

        template <class T>
        using ComponentList = std::list<T*>;

        Base();
        ~Base();

        virtual void update(float dt) {}
        virtual void destroy();

        bool isDestroyed() const { return _destroyed; }

        std::string toString() const;

    private:
        bool _destroyed;
    };
}

#endif
