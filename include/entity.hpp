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

#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "defines.hpp"

#include <memory>
#include <sstream>

#include "events/base.hpp"
#include "stringable.hpp"

#ifdef _USE_NEW_UUID
#   include "uuid.hpp"
#else
#   include <boost/functional/hash.hpp>
#   include <boost/uuid/uuid.hpp>
#   include <boost/uuid/uuid_io.hpp> // for uuid ostream operator
#endif

#ifdef _DEBUG
//#   define _DEBUG_ENTITIES
#endif

// Conceptually, an Entity should be nothing more than an identifier (an
// int, a string, etc). BUT, for convenience, make it a class with absolutely
// no additional functionality other than wrapper methods to EntityManager
class Entity final : public Stringable
{
    friend class EntityManager;

public:
#ifdef _USE_NEW_UUID
    typedef uuid::uuid UUID;
#else
    typedef boost::uuids::uuid UUID;
#endif

    static std::shared_ptr<Entity> create(const std::string &debugName);

private:
    Entity(UUID uuid, const std::string &debugName);

public:
    ~Entity();

    inline UUID getUUID() const { return _uuid; }
    inline std::string getDebugName() const { return _debugName; }

    std::string toString() const;

private:
    UUID _uuid;
    std::string _debugName;
};

namespace Events
{
    class EntityCreated : public Events::Base
    {
    public:
        std::shared_ptr<Entity> entity;

        EntityCreated(std::shared_ptr<Entity> entity_) :
            Events::Base(),
            entity(entity_) {}

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::EntityCreated[entity = " << entity << "]";
            return ss.str();
        }
    };
}

#endif
