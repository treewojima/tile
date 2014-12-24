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

#ifndef __RESOURCEMANAGER_HPP__
#define __RESOURCEMANAGER_HPP__

#include "defines.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include "exception.hpp"

template <class T>
class ResourceManager
{
public:
    typedef std::shared_ptr<T> ResourcePtr;

    ~ResourceManager();

    void add(const std::string &name, ResourcePtr ptr);

private:
    typedef std::unordered_map<std::string, ResourcePtr> ResourceMap;

    ResourceMap _map;
};

template <class T>
ResourceManager<T>::~ResourceManager()
{
    // NOTE: Should this just rely on clearing the map, or should it be ensured
    //       that every Resource is dropped?

    _map.clear();
}

template <class T>
void ResourceManager<T>::add(const std::string &name, ResourcePtr ptr)
{
    // TODO: Check if ptr is null

    auto ret = _map.insert(std::make_pair(name, ptr));
    if (!ret.second)
    {
        // NOTE: Should this be a logged warning instead?

        std::ostringstream ss;
        ss << "Could not insert new Resource \"" << name
           << "\" into ResourceManager: duplicate key";
        throw Exception(ss.str());
    }
}

#endif
