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

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include "defines.hpp"

#include <boost/variant.hpp>
#include <string>

//#include "entity.hpp" // for Entity::Type
enum class Type;

// Keep this updated with various types as needed
typedef boost::variant<std::string, bool, int, Type> Variant;

#endif
