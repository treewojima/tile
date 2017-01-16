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

#ifndef __ACTIONS_BASE_HPP__
#define __ACTIONS_BASE_HPP__

#include "stringable.hpp"

namespace Actions
{
    class Base : public Stringable
    {
    protected:
        Base(const std::string &&debugStr = "action") : _debugStr(std::move(debugStr)) {}

        void setDebugStr(const std::string &&str) { _debugStr = std::move(str); }

    public:
        virtual ~Base() {}

        std::string toString() const;

    private:
        std::string _debugStr;
    };
}

#endif
