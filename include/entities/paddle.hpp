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

#ifndef __PADDLE_HPP__
#define __PADDLE_HPP__

#include "defines.hpp"
#include "entity.hpp"
#include "game.hpp"

class Walls;

class Paddle : public Entity
{
public:
    Paddle(const b2Vec2 &position, std::shared_ptr<Walls> walls);
    ~Paddle();

    void draw();

    inline b2Vec2 getPosition() const { return _body->GetPosition(); }
    inline b2Vec2 getDimensions() const { return DIMENSIONS; }

    std::string toString() const;

private:
    static const b2Vec2 DIMENSIONS;
    enum { PADDLE_LEFT, PADDLE_RIGHT, NUM_PADDLE_EVENTS };

    b2Body *_body;
    b2Fixture *_fixture;
    Game::Event::Handle _eventHandles[NUM_PADDLE_EVENTS];

    void registerEvents();
};

#endif
