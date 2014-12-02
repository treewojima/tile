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
#include "level.hpp"

#include <boost/tokenizer.hpp>
#include <Box2D/Box2D.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "entities/block.hpp"
#include "exception.hpp"
#include "game.hpp"

Level::Level(const std::string &filename)
{
    loadFile(filename);
}

Level::~Level()
{
    _entities.clear();
}

void Level::update(float dt)
{
    for (auto entity : _entities)
    {
        entity->update(dt);
    }
}

void Level::draw()
{
    for (auto entity : _entities)
    {
        entity->draw();
    }
}

void Level::cullDeadEntities()
{
    _entities.remove_if([](std::shared_ptr<Entity> e)
    {
        return e->isMarkedForDeath();
    });
}

void Level::loadFile(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::ostringstream ss;
        ss << "Could not open level file: " << filename;
        throw Exception(ss.str());
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    for (const auto &line : lines)
    {
        boost::tokenizer<boost::escaped_list_separator<char>> tokens(line);
        auto iter = tokens.begin();

        auto name = *iter++;
        auto health = std::stoi(*iter++);
        auto row = std::stof(*iter++);
        auto col = std::stof(*iter++);

        const auto camera = Game::getCamera();
        b2Vec2 position(camera.getWorldWidth() / 8 * col,
                        camera.getWorldHeight() - (camera.getWorldHeight() / 16 * row));

        _entities.push_back(std::make_shared<Block>(name, position, health));
    }
}
