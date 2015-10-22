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

#include <sstream>

Components::PropertyList::PropertyList(const std::string &name) :
    Base(name)
{
}

Components::PropertyList::~PropertyList()
{
    _propMgr.clear();
}

void Components::PropertyList::add(const std::string &name, AnyType &value)
{
    _propMgr.add(name, DummyContainer<AnyType>(value));
}

bool Components::PropertyList::has(const std::string &name) const
{
    return _propMgr.has(name);
}

Components::PropertyList::AnyType
Components::PropertyList::get(const std::string &name) const
{
    return _propMgr.get(name).value;
}

void Components::PropertyList::clear()
{
    _propMgr.clear();
}

Components::PropertyList::AnyType
&Components::PropertyList::operator[](const std::string &key)
{
    if (has(key))
    {
        return get(key);
    }
    else
    {

    }
}

const Components::PropertyList::AnyType
&Components::PropertyList::operator[](const std::string &key) const
{

}

std::string Components::PropertyList::toString() const
{
    std::ostringstream ss;
    ss << "Components::PropertyList[name = \"" << getName()
       << "\", properties = { ";

    // HACK ALERT: this is to get around calling a non-const method from
    // a const instance, since we know that the lambda we pass will NOT
    // alter anything
    auto &ref = _propMgr;
    const_cast<PropertyManager&>(ref).forEachPair([&ss](auto pair)
    {
        const auto &key = pair.first;
        const auto &value = pair.second.value;
        ss << "\"" << key << "\" : " << value << ", ";
    });
    ss << "<end> }]";
    return ss.str();
}
