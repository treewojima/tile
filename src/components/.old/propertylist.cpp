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

#include "components/propertylist.hpp"

#ifndef _USE_NEW_COMPONENTS

#include <sstream>

Components::PropertyList::PropertyList(const std::string &name) :
    Base(name),
	ResourceManager<Variant>()
{
}

std::string Components::PropertyList::toString() const
{
    std::ostringstream ss;
    ss << "Components::PropertyList[name = \"" << getName()
       << "\", properties = { ";

    forEach([&ss](auto pair)
    {
        const auto &key = pair.key;
        const auto &value = pair.value;

		ss << "\"" << key << "\" : ";
		
		switch (value.which())
		{
		// std::string
		case 0:
			ss << "\"" << value << "\"";
			break;

		// bool
		case 1:
			ss << (boost::get<bool>(value) ? "true" : "false");
			break;

		default:
			ss << value;
		}

        ss << ", ";
    });

    ss << "<end> }]";
    return ss.str();
}

#endif
