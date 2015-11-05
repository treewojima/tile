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

#ifdef _USE_NEW_ENTITY
#   include "new_entity.hpp"
#else

#include <memory> 
#include <ostream>

#include "components/graphics.hpp"
#include "components/position.hpp"
#include "components/propertylist.hpp"
#include "stringable.hpp"

class Entity : public Stringable
{
public:
    Entity(const std::string &name);
    virtual ~Entity();

    std::shared_ptr<Components::Position> position;
    std::shared_ptr<Components::Graphics> graphics;
    std::shared_ptr<Components::PropertyList> properties;

    virtual void update(float dt) {}

    inline std::string getName() const { return _name; }
    inline bool isMarkedForDeath() const { return _markedForDeath; }

    std::string toString() const;

protected:
    // This is the most badass function name ever
    inline void markForDeath() { _markedForDeath = true; }

private:
    std::string _name;
    bool _markedForDeath;

public:
	enum class Type
	{
		Terrain,
		Actor
	};
};

// Helper stream operator for Entity::Type
inline std::ostream &operator<<(std::ostream &os, const Entity::Type &type)
{
	switch (type)
	{
	case Entity::Type::Terrain:
		os << "Terrain";
		break;

	case Entity::Type::Actor:
		os << "Actor";
		break;
	}

	return os;
}

#endif

#endif
