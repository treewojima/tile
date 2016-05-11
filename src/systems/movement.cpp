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

#include "systems/movement.hpp"

#include "components/position.hpp"
#include "game.hpp"
#include "logger.hpp"
#include "map/map.hpp"

Systems::Movement::Movement()
{
    _timer.start();
}

Systems::Movement::~Movement()
{
    destroy();
}

void Systems::Movement::update(float dt)
{
    if (_timer.getTicks() < 1000) return;

    for (EntityMovementMap::value_type &pair : _movements)
    {
        MovementQueue &queue = pair.second;
        if (!queue.empty())
        {
            Actions::Movement &m = queue.front();

            Vector2i &current = m.current;
            const Vector2i &dest = m.dest;

            // First, calculate the new position values
            // NOTE: This may be better accomplished through vector addition/subtraction
            const int magnitude = 1;
            for (int i = 0; i < 2; i++)
            {
                if (current[i] != dest[i])
                {
                    current[i] -= magnitude - (current[i] < dest[i] ? 2 : 0) * magnitude;
                }
            }
            LOG_DEBUG << "updated movement action: " << m;

            // Next, update the actual position components
            // NOTE: this is clunky code and should be changed
            auto pos = getGame().getEntityMgr().getComponent<Components::Position>(
                        m.entity->getUUID());
            auto mapPos = getGame().getEntityMgr().getComponent<Components::MapPosition>(
                        m.entity->getUUID());
            mapPos->x = current.x;
            mapPos->y = current.y;
            pos->x = (current.x + 1) * Map::Map::TILE_WIDTH - (Map::Map::TILE_WIDTH / 2);
            pos->y = (current.y + 1) * Map::Map::TILE_HEIGHT - (Map::Map::TILE_HEIGHT / 2);

            if (m.current == m.dest)
            {
                LOG_DEBUG << "finished movement action: " << m;
                queue.pop();
            }
        }
    }

    //dumpQueue();
    _timer.start();
}

void Systems::Movement::dumpQueue() const
{
    std::ostringstream ss;
    ss << "movement queue dump:";
#if 0
    int size = _movements.size();
    if (!size)
    {
        ss << " <empty>";
    }
    else
    {
        for (auto &m : _movementQueue)
        {
            ss << "\n    " << m;
        }
    }
#endif
    LOG_DEBUG << ss.str();
}

void Systems::Movement::destroy()
{
    if (isDestroyed()) return;

    _movements.clear();
    _timer.stop();

    Base::destroy();
}

void Systems::Movement::queueMovement(Actions::Movement &&movement)
{
    if (movement.origin != movement.dest)
    {
        LOG_DEBUG << "queueing movement action " << movement;
        _movements[std::move(movement.entity->getUUID())].push(std::move(movement));
    }
}

std::string Systems::Movement::toString() const
{
    std::ostringstream ss;
    ss << "Systems::Movement[movementQueueCount = " << _movements.size() << "]";
    return ss.str();
}
