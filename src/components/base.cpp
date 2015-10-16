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

#include "defines.hpp"
#include "components/base.hpp"
#include <sstream>

Components::Base::Base(const std::string &name) :
    _name(name)
{
}

Components::Base::~Base()
{
#ifdef _DEBUG_COMPONENTS
    BOOST_LOG_TRIVIAL(debug) << "destroyed component " << getName();
#endif
}

std::string Components::Base::toString() const
{
    std::ostringstream ss;
    ss << "Components::Base[name = \"" << getName() << "\"]";
    return ss.str();
}
