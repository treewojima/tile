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
#include <cstdint>
#include <easylogging++.h>
#include <string>

#include "colors.hpp"

class b2Fixture;

// Base virtual entity class
class Entity
{
public:
    // Entity types, for collision handling purposes
    enum Type
    {
        BARRIER = 0x1,
        PADDLE  = 0x2,
        BALL    = 0x4,
        BLOCK   = 0x8,
        CURSOR  = 0x10
    };

    // Constructor/destructor
    Entity(const std::string &name, Type type = Type::BARRIER);
    virtual ~Entity();

    // Update and draw functions, called in every tick of the loop
    virtual void update(float dt) {}
    virtual void draw() = 0;

    // Collision handlers
    virtual void startContact(const Entity *other, const b2Fixture *otherFixture) {}
    virtual void endContact(const Entity *other, const b2Fixture *otherFixture) {}

    // Various getters
    inline std::string getName() const { return _name; }
    inline Type getType() const { return _type; }
    inline bool isMarkedForDeath() const { return _markedForDeath; }
    virtual std::string toString() const;

protected:
    // This is the most badass function name ever
    inline void markForDeath() { _markedForDeath = true; }

private:
    std::string _name;
    Type _type;
    bool _markedForDeath;
};

// Helper stream operators
template <class T>
inline std::ostream &operator<<(std::ostream &stream, const Entity &e)
{
    stream << e.toString();
    return stream;
}

inline MAKE_LOGGABLE(Entity, e, stream)
{
    stream << e.toString();
    return stream;
}

#endif
