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
#include "components/position.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"
#include "exceptions.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "window.hpp"

const int Map::TILE_WIDTH = 32;
const int Map::TILE_HEIGHT = Map::TILE_WIDTH;

Map::Map(const std::string &filename) :
    _destroyed(false),
	_filename(filename)
{
    Events::Dispatcher::subscribe<Events::MapPositionComponentCreated>(*this);

    _map = tmx::Map::parseFile(filename);

    // Resize the internal grid
    _componentGrid.resize(_map->getWidth());
    _componentGrid.shrink_to_fit();
    for (auto &row : _componentGrid)
    {
        row.resize(_map->getHeight());
        row.shrink_to_fit();
    }

    loadTilesetTextures();

    LayerVisitor visitor(this);
    _map->visitLayers(visitor);
}

Map::~Map()
{
    if (!_destroyed) destroy();
}

void Map::destroy()
{
	if (_destroyed) return;

    Events::Dispatcher::unsubscribe(*this);

    //_entities.clear();
    //_components.clear();
    _componentGrid.clear();

    // NOTE: should components be unregistered here?

	_destroyed = true;
}

Map::ComponentList Map::getComponentsAt(int col, int row)
{
    try
    {
        return _componentGrid.at(col).at(row);
    }
    catch (std::out_of_range &e)
    {
        std::ostringstream ss;
        ss << __PRETTY_FUNCTION__ << ": map coordinates out of bounds: ("
           << col << ", " << row << ")";
        throw std::out_of_range(ss.str());
    }
}

void Map::onEvent(const Events::MapPositionComponentCreated &event)
{
    //_parent->_entities.push_back(entity);
    //_parent->_components.push_back(mapPos);

    auto c = event.component;
    int col = c->x, row = c->y;
    try
    {
        _componentGrid.at(col).at(row).push_back(c);
    }
    catch (std::out_of_range &e)
    {
        std::ostringstream ss;
        ss << __PRETTY_FUNCTION__ << ": map coordinates out of bounds: ("
           << col << ", " << row << ")";
        throw std::out_of_range(ss.str());
    }
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
    //const SDL_Color colorKey = Colors::makeColor(0xFF, 0, 0xFF);
    auto rgbStr = _map->getBackgroundColor().substr(1);
    const SDL_Color colorKey = Colors::parseRGBHexString(rgbStr);

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
            throw Exceptions::MapException(ss.str());
		}

		const int tileHeight = tileset->getTileHeight();
		if (tileHeight != TILE_HEIGHT)
		{
			std::ostringstream ss;
			ss << "Map " << _filename << " does not have a tile height of "
				<< TILE_HEIGHT << " (" << tileHeight << " instead)";
            throw Exceptions::Base(ss.str());
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
        //bool passable = (layer.getProperty("passable", "true") == "true" ? true : false);

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

            // Create the entity
            std::ostringstream entityName;
            entityName << layerName << "-" << col << "-" << row
                       << "-" << textureName.str();
			auto entity = Entity::create(entityName.str());

            auto mapPos = Components::MapPosition::create(entity,
                                                          col,
                                                          map.getHeight() - row - 1);

            auto absPos = Components::Position::create(entity, *mapPos);

            auto texture = Game::getTexMgr().get(textureName.str());
            //entity->graphics =
            //        std::make_shared<Components::StaticSprite>(texture,
            //                                                   entity->position);
			Components::Graphics::Sprite::create(entity, textureName.str());
//#define _DEBUG_ENTITIES
#if defined(_DEBUG_MAP) && defined(_DEBUG_ENTITIES)
            LOG_DEBUG << "created entity: (" << mapPos->x << "," << mapPos->y << ") -> "
                      << "(" << absPos->x << "," << absPos->y << ")";
#endif
        }

        cellID++;
    }
}
