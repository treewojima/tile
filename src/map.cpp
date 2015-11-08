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

#include "map.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <tmx/Cell.h>
#include <tmx/TileLayer.h>

#include "colors.hpp"
#include "components/mapposition.hpp"
#include "components/position.hpp"
#include "entity.hpp"
#include "exception.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "window.hpp"

const int Map::TILE_WIDTH = 32;
const int Map::TILE_HEIGHT = Map::TILE_WIDTH;

Map::Map(const std::string &filename) :
    _destroyed(false),
	_filename(filename)
{
    _map = tmx::Map::parseFile(filename);

    loadTilesetTextures();

    LayerVisitor visitor(this);
    _map->visitLayers(visitor);

#if 0
    _map = new Tmx::Map();
    _map->ParseFile(filename);
    if (_map->HasError())
    {
        std::ostringstream ss;
        ss << "Error " << static_cast<unsigned>(_map->GetErrorCode())
           << " while loading Map \"" << _name << "\": " << _map->GetErrorText();
        delete _map;
        throw MapException(ss.str());
    }
#endif
}

Map::~Map()
{
    if (!_destroyed) destroy();
}

void Map::destroy()
{
	if (_destroyed) return;

	_entities.clear();

	_destroyed = true;
}

std::string Map::toString() const
{
	std::ostringstream ss;
	ss << "Map[filename = \"" << _filename << "\"]";
	return ss.str();
}

void Map::loadTilesetTextures()
{
    // NOTE: This is very, very hacky and ignores the settings in the map file
    const SDL_Color colorKey = Colors::makeColor(0xFF, 0, 0xFF);

    for (auto tileset : _map->getTileSets())
    {
        auto image = tileset->getImage();
        auto tilesetSurface = Graphics::loadSDLSurface(image->getSource().string());

        const int tileWidth = tileset->getTileWidth();
		if (tileWidth != TILE_WIDTH)
		{
			std::ostringstream ss;
			ss << "Map " << _filename << " does not have a tile width of "
			   << TILE_WIDTH << " (" << tileWidth << " instead)";
			throw Exception(ss.str());
		}

		const int tileHeight = tileset->getTileHeight();
		if (tileHeight != TILE_HEIGHT)
		{
			std::ostringstream ss;
			ss << "Map " << _filename << " does not have a tile height of "
				<< TILE_HEIGHT << " (" << tileHeight << " instead)";
			throw Exception(ss.str());
		}

        const int tilesetWidth = tilesetSurface->w;
        const int tilesetHeight = tilesetSurface->h;
        const int numCols = tilesetWidth / tileWidth;
        const int numRows = tilesetHeight / tileHeight;
        const int margin = tileset->getMargin();

#if 0
        std::cout << "tileWidth     = "   << tileWidth
                  << "\ntileHeight    = " << tileHeight
                  << "\ntilesetWidth  = " << tilesetWidth
                  << "\ntilesetHeight = " << tilesetHeight
                  << "\nmargin        = " << margin
                  << "\nnumCols       = " << numCols
                  << "\nnumRows       = " << numRows
                  << "\nfirstGid      = " << tileset->getFirstGID() << std::endl;
#endif

        auto gid = tileset->getFirstGID();
        SDL_Rect srcRect;
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                srcRect.x = (margin * (col + 1)) + (tileWidth * col);
                srcRect.y = (margin * (row + 1)) + (tileHeight * row);
                srcRect.w = tileWidth;
                srcRect.h = tileHeight;

                auto destSurface = Graphics::createBlankSDLSurface(tileWidth, tileHeight);
                assert(destSurface != nullptr);

                SDL_BlitSurface(tilesetSurface,
                                &srcRect,
                                destSurface,
                                nullptr);

                std::ostringstream name;
                name << tileset->getName() << "-" << gid++;

                // NOTE: For some reason, freeing destSurface causes segfaults
                //       or similar errors. It seems to be random exactly which
                //       iteration of the loop it occurs, so I'm not sure what's
                //       causing it. It's odd because the surface optimization
                //       process frees an intermediate surface per iteration as
                //       well, but with no errors.
                //
                //       By setting freeSurface to false this issue is avoided.
                //       However, it leaves the intermediate surface in memory,
                //       which is a leak!
                auto texture = std::make_shared<Texture>(name.str(),
                                                         destSurface,
                                                         &colorKey,
                                                         false,       // do NOT free the surface
                                                         true);
                Game::getTexMgr().add(texture->getName(), texture);
            }
        }

        SDL_FreeSurface(tilesetSurface);
    }
}

void Map::LayerVisitor::visitTileLayer(const tmx::Map &map, const tmx::TileLayer &layer)
{
    if (!layer.isVisible()) return;

    auto layerName = layer.getName();
    unsigned cellID = 0;
    for (auto &cell : layer)
    {
		bool passable = (layer.getProperty("passable", "true") == "true" ? true : false);

        auto gid = cell.getGID();
        if (gid)
        {
            // Extrapolate the texture resource from the tileset name and cell GID
            auto tileset = map.getTileSetFromGID(gid);
            //gid -= tileset->getFirstGID();
            std::ostringstream textureName;
            textureName << tileset->getName() << "-" << gid;

            // Calculate the tile's position on the screen/within the map
            auto col = cellID % map.getWidth();
			assert(col < map.getWidth());
            auto row = cellID / map.getWidth();
            assert(row < map.getHeight());
            float x = col * map.getTileWidth();
            float y = row * map.getTileHeight();

            // Create the ***TEST*** entity
            std::ostringstream entityName;
            entityName << layerName << "-" << col << "-" << row
                       << "-" << textureName.str();
            //auto entity = std::make_shared<Entity>(entityName.str());
			auto entity = Entity::create(entityName.str());

			Vector2i v(1 + col, map.getHeight() - row);
			auto pos = Components::MapPosition::create(entity, v);

            //Vector2f pos(x + 50, Window::getHeight() - (y + 50));
            //entity->position =
            //        std::make_shared<Components::Position>(pos);
			auto pos2 = Components::Position::create(entity, *pos);

            auto texture = Game::getTexMgr().get(textureName.str());
            //entity->graphics =
            //        std::make_shared<Components::StaticSprite>(texture,
            //                                                   entity->position);
			Components::Graphics::Sprite::create(entity, textureName.str());
			
            _parent->_entities.push_back(entity);

			LOG_DEBUG << "created entity: (" << pos->x << "," << pos->y << ") -> "
				      << "(" << pos2->x << "," << pos2->y << ")";
        }

        cellID++;
    }
}
