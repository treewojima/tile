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

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "exception.hpp"
//#include "logger.hpp"

template <class T>
class KeyValuePair
{
public:
	KeyValuePair(const std::string &k, T &v) : key(k), value(v) {}
	KeyValuePair(const std::pair<std::string, T> &pair) : key(pair.first), value(pair.second) {}

	std::string key;
	T value;
};

// Lightweight wrapper around a map with strings as keys
template <class T>
class ResourceManager
{
private:
	typedef T ResourceType;

    // unordered_map is faster than map for random access, but slower
    // for sequential access - which is optimal for what the ResourceManager
    // is used for
    typedef std::unordered_map<std::string, ResourceType> ResourceMap;

public:
	ResourceManager() : _destroyed(false) {}
    virtual ~ResourceManager();
	void destroy();

    void add(const std::string &name, ResourceType resource);
	bool has(const std::string &name) const;
	ResourceType get(const std::string &name) const;
    void clear();

	ResourceType &operator[](const std::string &name);

	void forEach(std::function<void(KeyValuePair<ResourceType>)> f) const;

private:
	bool _destroyed;
    ResourceMap _map;
};

template <class T>
ResourceManager<T>::~ResourceManager()
{
	if (!_destroyed) destroy();
}

template <class T>
void ResourceManager<T>::destroy()
{
	// NOTE: Should this just rely on clearing the map, or should it be ensured
	//       that every ResourcePtr is dropped properly?

	_map.clear();

	_destroyed = true;
}

template <class T>
void ResourceManager<T>::add(const std::string &name, ResourceType resource)
{
#if 0
    if (!ptr)
    {
        std::ostringstream ss;
        ss << "could not insert new resource \"" << name
           << "\" into ResourceManager: NULL pointer";
        throw Exception(ss.str());
    }
#endif

    auto ret = _map.insert(std::make_pair(name, resource));
    if (!ret.second)
    {
        // NOTE: Should this be a logged warning instead?
        //       ... Yes, yes it should.

        std::ostringstream ss;
        ss << "could not insert new resource \"" << name
                  << "\" into ResourceManager: duplicate key";
        throw Exception(ss.str());
    }
}

template <class T>
bool ResourceManager<T>::has(const std::string &name) const
{
	return _map.count(name);
}

template <class T>
typename ResourceManager<T>::ResourceType ResourceManager<T>::get(const std::string &name) const
{
	// NOTE: Should this use has() or the subscript operator instead?

    try
    {
        return _map.at(name);
    }
    catch (std::out_of_range &e)
    {
        // For now, just rethrow the exception
        throw e;
    }
}

template <class T>
void ResourceManager<T>::clear()
{
    // NOTE: Should every ResourcePtr be properly dropped, or is clearing the
    //       internal map enough?

    _map.clear();
}

template <class T>
typename ResourceManager<T>::ResourceType &ResourceManager<T>::operator[](const std::string &name)
{
	return _map[name];
}

template <class T>
void ResourceManager<T>::forEach(std::function<void(KeyValuePair<ResourceType>)> f) const
{
    for (auto iter = _map.begin() ; iter != _map.end(); iter++)
    {
        f(KeyValuePair<ResourceType>(*iter));
    }
}

#endif
