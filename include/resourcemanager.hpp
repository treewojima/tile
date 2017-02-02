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

#ifndef __RESOURCEMANAGER_HPP__
#define __RESOURCEMANAGER_HPP__

#include "defines.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "exceptions.hpp"

namespace Exceptions
{
    class NoSuchResource : public Base
    {
    public:
        NoSuchResource(const std::string &name) :
            Base(std::string("no such resource \"") + name + std::string("\"")) {}
    };
}

#if 0
template <class V, class K = std::string>
class KeyValuePair
{
public:
    typedef K Key;
    typedef V Value;

    KeyValuePair(const Key &k, Value &v) : key(k), value(v) {}
    KeyValuePair(const std::pair<Key, Value> &pair) : key(pair.first), value(pair.second) {}

    Key key;
    Value value;
};
#endif

// Lightweight wrapper around a map
template <class V, class K = std::string>
class ResourceManager
{
public:
    typedef V Resource;
    typedef K Key;

private:
    // unordered_map is faster than map for random access, but slower
    // for sequential access - which is optimal for what the ResourceManager
    // is used for
    typedef std::unordered_map<Key, Resource> ResourceMap;

public:
    virtual ~ResourceManager();
    void destroy();

    void add(const Key &name, const Resource &resource);
    bool has(const Key &name) const;
    Resource get(const Key &name) const;
    void remove(const Key &name);
    void clear();

    Resource &operator[](const Key &name);

    void forEach(std::function<void (const std::pair<Key, Resource> &)> f) const;

private:
    ResourceMap _map;
};

template <class V, class K>
ResourceManager<V, K>::~ResourceManager()
{
    destroy();
}

template <class V, class K>
void ResourceManager<V, K>::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    // NOTE: Should this just rely on clearing the map, or should it be ensured
    //       that every ResourcePtr is dropped properly?

    clear();

    destroyed = true;
}

template <class V, class K>
void ResourceManager<V, K>::add(const Key &name, const Resource &resource)
{
    auto ret = _map.insert(std::make_pair(name, resource));
    if (!ret.second)
    {
        // NOTE: Should this be a logged warning instead?
        //       ... Yes, yes it should.

        std::ostringstream ss;
        ss << "could not insert new resource \"" << name
                  << "\" into ResourceManager: duplicate key";
        throw Exceptions::Base(ss.str());
    }
}

template <class V, class K>
bool ResourceManager<V, K>::has(const Key &name) const
{
    return _map.count(name);
}

template <class V, class K> typename ResourceManager<V, K>::Resource
ResourceManager<V, K>::get(const Key &name) const
{
    // NOTE: Should this use has() or the subscript operator instead?

    try
    {
        return _map.at(name);
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchResource(name);
    }
}

template <class V, class K>
void ResourceManager<V, K>::remove(const Key &name)
{
    auto i = _map.find(name);
    if (i == _map.end())
    {
        throw Exceptions::NoSuchResource(name);
    }
    else
    {
        _map.erase(i);
    }
}

template <class V, class K>
void ResourceManager<V, K>::clear()
{
    // NOTE: Should every ResourcePtr be properly dropped, or is clearing the
    //       internal map enough?

    _map.clear();
}

template <class V, class K> typename ResourceManager<V, K>::Resource
&ResourceManager<V, K>::operator[](const Key &name)
{
    try
    {
        return _map.at(name);
    }
    catch (std::out_of_range)
    {
        throw Exceptions::NoSuchResource(name);
    }
}

template <class V, class K>
void ResourceManager<V, K>::forEach(std::function<void (const std::pair<Key, Resource> &)> f) const
{
    //for (const auto &iter = _map.begin() ; iter != _map.end(); iter++)
    for (const auto &pair : _map)
    {
        f(pair);
    }
}

#endif
