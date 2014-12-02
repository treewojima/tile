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

#include "defines.hpp"
#include "entities/block.hpp"

#include <cassert>
#include <GL/glew.h>
#include "physics.hpp"

const b2Vec2 Block::DIMENSIONS = b2Vec2(1.5, 0.5);

namespace
{
    const SDL_Color COLORS[] = { Colors::BLUE,
                                 Colors::RED,
                                 Colors::GREEN };
}

Block::Block(const std::string &name, const b2Vec2 &position, int health) :
    Entity(name, Type::BLOCK),
    _body(nullptr),
    _fixture(nullptr),
    _health(health),
    _color(COLORS[health - 1])
{
    // First, create the body itself
    b2BodyDef bodyDef;
    bodyDef.position = position;
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;
    _body = Physics::createBody(bodyDef);

    // Now create the body fixture
    b2PolygonShape box;
    box.SetAsBox(DIMENSIONS.x / 2, DIMENSIONS.y / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.filter.categoryBits = getType();
    fixtureDef.filter.maskBits = Type::BARRIER | Type::BALL | Type::CURSOR;
    _fixture = _body->CreateFixture(&fixtureDef);
}

Block::~Block()
{
    assert(_body != nullptr);
    Physics::destroyBody(_body);
}

void Block::draw()
{
    auto position = getPosition();

    // Adjust for coordinates origin
    position.x -= DIMENSIONS.x / 2.f;
    position.y -= DIMENSIONS.y / 2.f;

    // Draw a filled rectangle
    glColor4ub(_color.r, _color.g, _color.b, _color.a);
    glRectf(position.x,
            position.y,
            position.x + DIMENSIONS.x,
            position.y + DIMENSIONS.y);
}

void Block::startContact(const Entity *other, const b2Fixture *otherFixture)
{
    assert(_health >= 0);

    if (other->getName() == "Ball")
    {
        _health--;
        _color = COLORS[_health - 1];
    }

    if (!_health)
    {
        markForDeath();
    }
}

std::string Block::toString() const
{
    std::ostringstream ss;
    ss << "Block[name = \"" << getName() << "\", position = " << getPosition()
       << ", dimensions = " << getDimensions() << "]";
    return ss.str();
}
