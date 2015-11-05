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

#ifndef __COMPONENTS_BASE_HPP__
#define __COMPONENTS_BASE_HPP__

#include "defines.hpp"

#ifdef _USE_NEW_COMPONENTS
#   include "components/new_base.hpp"
#else

#include <memory>
#include <string>
#include "stringable.hpp"

namespace Components
{
    class Base : public Stringable
    {
    public:
        //typedef std::shared_ptr<Base> Ptr;

        Base(const std::string &name);
        virtual ~Base();

        inline std::string getName() const { return _name; }

        std::string toString() const;

    private:
        std::string _name;
    };
}

#endif

#endif
