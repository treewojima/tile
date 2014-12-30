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

// Base virtual entity class
class Entity
{
public:

    // Constructor/destructor
    Entity(const std::string &name);
    virtual ~Entity();

    // Update and draw functions, called in every tick of the loop
    virtual void update(float dt) {}
    virtual void draw() = 0;

    // Various getters
    inline std::string getName() const { return _name; }
    inline bool isMarkedForDeath() const { return _markedForDeath; }
    virtual std::string toString() const;

protected:
    // This is the most badass function name ever
    inline void markForDeath() { _markedForDeath = true; }

private:
    std::string _name;
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
