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

#include "map/tileset.hpp"
#include "map/map.hpp"

Map::Tileset::Tileset(const tmx::TileSet *tileset) : _tileset(tileset)
{
    assert(tileset);
    _surface = Graphics::Surface::create(_tileset->getImage()->getSource().string());

    const unsigned numCols = _surface->getWidth() / Map::TILE_WIDTH;
    const unsigned numRows = _surface->getHeight() / Map::TILE_HEIGHT;
    const unsigned margin = _tileset->getMargin();

    // Loop through tileset and calculate blitting rects for each individual tile
    SDL_Rect rect;
    rect.w = Map::TILE_WIDTH;
    rect.h = Map::TILE_HEIGHT;
    for (unsigned row = 0; row < numRows; row++)
    {
        for (unsigned col = 0; col < numCols; col++)
        {
            rect.x = (margin * (col + 1)) + (Map::TILE_WIDTH * col);
            rect.y = (margin * (row + 1)) + (Map::TILE_HEIGHT * row);

            _rects.push_back(rect);
        }
    }
}

Map::Tileset::~Tileset()
{
    destroy();
}

void Map::Tileset::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    delete _surface;

    destroyed = true;
}

const SDL_Rect &Map::Tileset::getRect(unsigned gid)
{
    assert(_tileset);
    //return _rects[(gid - 1) - (_tileset->getFirstGID() - 1)];
    try
    {
        return _rects.at(gid - 1);
    }
    catch (std::out_of_range &e)
    {
        throw Exceptions::MapException(e.what());
    }
}
