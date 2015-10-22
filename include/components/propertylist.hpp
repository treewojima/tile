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

#ifndef __COMPONENTS_PROPERTYLIST_HPP__
#define __COMPONENTS_PROPERTYLIST_HPP__

#include "defines.hpp"

//#include <boost/any.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>

#include "components/base.hpp"
#include "resourcemanager.hpp"

// TODO: swap out ResourceManager for unordered_map; the abstraction
// makes it near impossible to subscript... OR, tweak ResourceManager
// to allow for proper subscripting itself

namespace Components
{
    class PropertyList : public Base
    {
    public:
        typedef boost::spirit::hold_any AnyType;

        //using Base::Base;
        PropertyList(const std::string &name = "PropertyList");
        ~PropertyList();

        void add(const std::string &name, AnyType &value);
        bool has(const std::string &name) const;
        AnyType get(const std::string &name) const;
        void clear();

        AnyType &operator[](const std::string &key);
        const AnyType &operator[](const std::string &key) const;

        std::string toString() const;

    private:
        // Dummy container for ResourceManager
        template <class T>
        class DummyContainer
        {
        public:
            DummyContainer(T &v) : value(v) {}

            T value;
        };

        typedef ResourceManager<AnyType, DummyContainer> PropertyManager;

        PropertyManager _propMgr;
    };
}

#endif
