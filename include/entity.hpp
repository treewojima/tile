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

#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "defines.hpp"

#include <boost/core/demangle.hpp>
#include <memory>
#include <sstream>

#include "events/base.hpp"
#include "pool.hpp"
#include "stringable.hpp"
#include "uuid.hpp"

#ifdef _DEBUG
#   define _DEBUG_ENTITIES
#endif

namespace Components { class Base; }

// Conceptually, an Entity should be nothing more than an identifier (an
// int, a string, etc). BUT, for convenience, make it a class with absolutely
// no additional functionality other than wrapper methods to EntityManager
static const unsigned ENTITY_POOL_CAPACITY = 10;
class Entity final :
        public Stringable,
        public PoolableObject<Entity, ENTITY_POOL_CAPACITY>
{
    friend class EntityManager;

public:
    typedef uuid::uuid UUID;

    static Entity *create(const std::string &debugName);

private:
    Entity(UUID uuid, const std::string &debugName);

public:
    ~Entity();

    inline UUID getUUID() const { return _uuid; }
    inline std::string getDebugName() const { return _debugName; }

    bool dirty;

    // Helper method for chaining component creation calls
    template <class C, class... Args>
    Entity *component(Args&& ... args)
    {
        static_assert(std::is_base_of<Components::Base, C>::value,
                      "Can only add components of a type derived from class Components::Base");

        C::create(_uuid, std::forward<Args>(args)...);

        return this;
    }

    std::string toString() const;

protected:
    //inline void setDebugName(const std::string &name) { _debugName = name; }
    inline void setDebugName(std::string &&name) { _debugName = std::move(name); }

private:
    UUID _uuid;
    std::string _debugName;
};

// The component base class is in this file to prevent cyclic preprocessor includes
namespace Components
{
    static const unsigned COMPONENT_POOL_CAPACITY = 50;
    class Base :
            public Stringable,
            public PoolableObject<Base, COMPONENT_POOL_CAPACITY>
    {
    protected:
        Base(const Entity::UUID &parentUUID, const std::string &debugName = "Component");

    public:
        virtual ~Base();

        inline std::string getDebugName() const { return _debugName; }
        std::shared_ptr<Entity> getParent() const;

        std::string toString() const;

    protected:
        //inline void setDebugName(const std::string &name) { _debugName = name; }
        inline void setDebugName(std::string &&name) { _debugName = std::move(name); }

    private:
        Entity::UUID _parentUUID;
        std::string _debugName;
    };
}

// Entity and component events
namespace Events
{
    class EntityCreated : public Events::Base
    {
    public:
        Entity::UUID uuid;

        EntityCreated(Entity::UUID uuid_) :
            Events::Base(),
            uuid(uuid_) {}

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::EntityCreated[uuid = " << uuid << "]";
            return ss.str();
        }
    };

    class EntityDestroyed : public Events::Base
    {
    public:
        Entity::UUID uuid;

        EntityDestroyed(Entity::UUID uuid_) :
            Events::Base(),
            uuid(uuid_) {}


        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::EntityDestroyed[uuid = " << uuid << "]";
            return ss.str();
        }
    };

    template <class T>
    class SpecificComponentCreated : public Events::Base
    {
    public:
        T *component;

        SpecificComponentCreated(T *component_) :
            Events::Base(),
            component(component_) {}

        std::string toString() const
        {
            std::ostringstream ss;
            ss << "Events::SpecificComponentCreated<"
               << boost::core::demangle(typeid(T).name()) << ">"
               << "[component = " << component << "]";
            return ss.str();
        }
    };

    typedef SpecificComponentCreated<Components::Base> ComponentCreated;
}

#endif
