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
#include "entities/walls.hpp"

#include "game.hpp"
#include "physics.hpp"

Walls::Walls() :
    Entity("Walls"),
    _body(nullptr),
    _bottomFixture(nullptr)
{
    b2BodyDef bodyDef;
    bodyDef.position.SetZero();
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;
    _body = Physics::createBody(bodyDef);

    b2EdgeShape edge;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &edge;

    const float WORLD_WIDTH = Game::getCamera().getWorldWidth();
    const float WORLD_HEIGHT = Game::getCamera().getWorldHeight();

    // Bottom wall/fixture
    edge.Set(b2Vec2_zero, b2Vec2(WORLD_WIDTH, 0));
    _bottomFixture = _body->CreateFixture(&fixtureDef);

    // Other walls
    edge.Set(b2Vec2_zero, b2Vec2(0, WORLD_HEIGHT));
    _body->CreateFixture(&fixtureDef);
    edge.Set(b2Vec2(0, WORLD_HEIGHT),
            b2Vec2(WORLD_WIDTH, WORLD_HEIGHT));
    _body->CreateFixture(&fixtureDef);
    edge.Set(b2Vec2(WORLD_WIDTH, WORLD_HEIGHT),
            b2Vec2(WORLD_WIDTH, 0));
    _body->CreateFixture(&fixtureDef);
}

Walls::~Walls()
{
    assert(_body != nullptr);
    Physics::destroyBody(_body);
}
