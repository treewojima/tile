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

#include "map/map.hpp"

#include "components/position.hpp"
#include "components/sprite.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "tmx/Cell.h"
#include "tmx/TileLayer.h"

const int Map::Map::TILE_WIDTH = 32;
const int Map::Map::TILE_HEIGHT = 32;

inline std::ostream &operator<<(std::ostream &os, const SDL_Rect &rect)
{
    os << "SDL_Rect { " << rect.x << ", " << rect.y << ", "
       << rect.w << ", " << rect.h << " }";
    return os;
}

Map::Map::Map(const std::string &filename)
{
    _map = tmx::Map::parseFile(filename);

    loadTilesets();

    LayerVisitor visitor(this);
    _map->visitLayers(visitor);
}

Map::Map::~Map()
{
    _tilesets.clear();
    _map.reset();
}

void Map::Map::loadTilesets()
{
    for (const tmx::TileSet *tileset : _map->getTileSets())
    {
        _tilesets[tileset->getName()] = std::make_shared<Tileset>(tileset);
    }
}

void Map::Map::LayerVisitor::visitTileLayer(const tmx::Map &map,
                                            const tmx::TileLayer &layer)
{
    if (!layer.isVisible()) return;

    const unsigned mapWidth = map.getWidth();
    const unsigned mapHeight = map.getHeight();

    auto layerSurface =
            Graphics::Surface::create(Vector2i(mapWidth * TILE_WIDTH,
                                               mapHeight * TILE_HEIGHT),
                                      Color::COLOR_KEY);

    unsigned cellid = 0;
    for (auto i = layer.begin();
         i != layer.end();
         i++, cellid++)
    {
        unsigned gid = (*i).getGID();
        if (!gid) continue;

        auto tmxTileset = map.getTileSetFromGID(gid);
        auto tileset = _parent->_tilesets.at(tmxTileset->getName());

        SDL_Rect srcRect = tileset->getRect(gid);
        SDL_Rect destRect = { static_cast<int>(cellid % mapWidth) * TILE_WIDTH,
                              static_cast<int>(cellid / mapWidth) * TILE_HEIGHT,
                              TILE_WIDTH,
                              TILE_HEIGHT };

        //LOG_PLAIN << "blitting gid " << gid << ": " << srcRect << " -> " << destRect;

        Graphics::Surface::blit(tileset->getSurface(),
                                &srcRect,
                                layerSurface,
                                &destRect);
    }

    Graphics::Texture::create(layer.getName(), layerSurface);

    auto layerName = layer.getName();
    Entity::create(layerName)->component<Components::Position>()
                             ->component<Components::Sprite>(layerName);
}
