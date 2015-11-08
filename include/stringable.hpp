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

#ifndef __ISTRINGABLE_HPP__
#define __ISTRINGABLE_HPP__

#include "defines.hpp"

#include <memory>
#include <string>
#include <type_traits>

// Interface that guarantees a toString() method
class Stringable
{
public:
    virtual ~Stringable() {}
    virtual std::string toString() const = 0;
};

// Helper ostream operators
inline std::ostream &operator<<(std::ostream &stream, const Stringable &obj)
{
    stream << obj.toString();
    return stream;
}

template <class T>
inline std::ostream &operator<<(std::ostream &os, const std::shared_ptr<T> &ptr)
{
	static_assert(std::is_base_of<Stringable, T>::value,
				  "Class T is not subclass of type Stringable");
			
	os << ptr->toString();
	return os;
}

template <class T>
inline std::ostream &operator<<(std::ostream &os, const std::unique_ptr<T> &ptr)
{
	static_assert(std::is_base_of<Stringable, T>::value,
			      "Class T is not subclass of type Stringable");

	os << ptr->toString();
	return os;
}

#endif