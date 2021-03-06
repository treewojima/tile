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

#include "defines.hpp"
#include "states/maingame.hpp"

#include <sstream>

#include "color.hpp"
#include "entities/dog.hpp"
#include "game.hpp"
#include "logger.hpp"
#include "systems/graphics.hpp"

void States::MainGame::initialize()
{
    LOG_DEBUG << "initialized States::MainGame";

    _map = new Map::Map("res/desert.tmx");

    loadTextures();
    createEntities();
}

void States::MainGame::destroy()
{
    delete _map;
    LOG_DEBUG << "destroyed States::MainGame";
}

void States::MainGame::draw(float dt)
{
    // First, draw entities
    getGame().getGraphicsSys().update(dt);

    // Draw a grid overlay signifying tile boundaries
    const auto &screenSize = getGame().getWindow().getDimensions();
    const auto &tileSize = Map::Map::TILE_DIMENSIONS;
    Vector2i overlaySize(screenSize.x - screenSize.x % tileSize.x,
                         screenSize.y - screenSize.y % tileSize.y);

    for (int x = 0; x <= overlaySize.x; x += tileSize.x)
    {
        getGame().getRenderer().drawLine(Color::WHITE,
                                         x, 0,
                                         x, overlaySize.y);
    }
    for (int y = 0; y <= overlaySize.y; y += tileSize.y)
    {
        getGame().getRenderer().drawLine(Color::WHITE,
                                         0, y,
                                         overlaySize.x, y);
    }
}

void States::MainGame::loadTextures()
{
}

void States::MainGame::createEntities()
{
    createDog();
}

