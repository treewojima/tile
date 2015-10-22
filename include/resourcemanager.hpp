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

template <class T, template <class> class Container = std::shared_ptr>
class ResourceManager
{
public:
    typedef Container<T> ResourcePtr;
    typedef Container<const T> ConstResourcePtr;

private:
    // unordered_map is faster than map for random access, but slower
    // for sequential access - which is optimal for what the ResourceManager
    // is used for
    typedef std::unordered_map<std::string, ResourcePtr> ResourceMap;

public:
	ResourceManager() : _destroyed(false) {}
    virtual ~ResourceManager();
	void destroy();

    void add(const std::string &name, ResourcePtr ptr);
	bool has(const std::string &name) const;
    ResourcePtr get(const std::string &name) const;
    void clear();

    //pair<const key_type,mapped_type>
    void forEachPair(std::function<void(typename ResourceMap::value_type)> f);
    void forEach(std::function<void(ResourcePtr)> f);

private:
	bool _destroyed;
    ResourceMap _map;
};

template <class T, template <class> class C>
ResourceManager<T, C>::~ResourceManager()
{
	if (!_destroyed) destroy();
}

template <class T, template <class> class C>
void ResourceManager<T, C>::destroy()
{
	// NOTE: Should this just rely on clearing the map, or should it be ensured
	//       that every ResourcePtr is dropped properly?

	_map.clear();

	_destroyed = true;
}

template <class T, template <class> class C>
void ResourceManager<T, C>::add(const std::string &name, ResourcePtr ptr)
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

    auto ret = _map.insert(std::make_pair(name, ptr));
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

template <class T, template <class> class C>
bool ResourceManager<T, C>::has(const std::string &name) const
{
	return _map.count(name);
}

template <class T, template <class> class C>
typename ResourceManager<T, C>::ResourcePtr ResourceManager<T, C>::get(const std::string &name) const
{
	// NOTE: Should this use has() instead?

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

template <class T, template <class> class C>
void ResourceManager<T, C>::clear()
{
    // NOTE: Should every ResourcePtr be properly dropped, or is clearing the
    //       internal map enough?

    _map.clear();
}

template <class T, template <class> class C>
void ResourceManager<T, C>::forEachPair(std::function<void(typename ResourceMap::value_type)> f)
{
    for (auto iter = _map.begin() ; iter != _map.end(); iter++)
    {
        f(*iter);
    }
}

template <class T, template <class> class C>
void ResourceManager<T, C>::forEach(std::function<void(ResourcePtr)> f)
{
#if 0
    for (auto iter = _map.begin() ; iter != _map.end(); iter++)
    {
        f((*iter).second);
    }
#endif
    forEachPair([&f](auto pair)
    {
        f(pair.second);
    });
}

#endif
