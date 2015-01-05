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

#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "defines.hpp"
#include <easylogging++.h>
#include <memory>
#include <string>

namespace Components
{
    class Base
    {
    public:
        //typedef std::shared_ptr<Base> Ptr;

        Base(const std::string &name);
        virtual ~Base();

        inline std::string getName() const { return _name; }

        virtual std::string toString() const;

    private:
        std::string _name;
    };
}

// Helper stream operators
template <class T>
inline std::ostream &operator<<(std::ostream &stream,
                                const Components::Base &c)
{
    stream << c.toString();
    return stream;
}

inline MAKE_LOGGABLE(Components::Base, c, stream)
{
    stream << c.toString();
    return stream;
}

#endif
