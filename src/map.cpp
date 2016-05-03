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
#include <SDL2/SDL_image.h>
#include <sstream>
#include <tmx/Cell.h>
#include <tmx/TileLayer.h>

#include "color.hpp"
#include "components/position.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"
#include "exceptions.hpp"
#include "game.hpp"
//#include "graphics.hpp"
//#include "window.hpp"

const int Map::TILE_WIDTH = 32;
const int Map::TILE_HEIGHT = Map::TILE_WIDTH;

Map::Map(const std::string &filename) :
	_filename(filename)
{
    Events::Dispatcher::subscribe<Events::MapPositionComponentCreated>(*this);

    _map = tmx::Map::parseFile(filename);

    // Resize the internal grid
    _entityGrid.resize(_map->getWidth());
    _entityGrid.shrink_to_fit();
    for (auto &row : _entityGrid)
    {
        row.resize(_map->getHeight());
        row.shrink_to_fit();
    }

    loadTilesetTextures();

    LayerVisitor visitor(this);
    _map->visitLayers(visitor);

    // Create the preprocessed map texture
    preprocess();
}

Map::~Map()
{
    destroy();
}

void Map::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

    Events::Dispatcher::unsubscribe(*this);

    _entityGrid.clear();
    getGame().getTexMgr().remove(_preprocessedTexture->getName());
    _preprocessedTexture.reset();

    // NOTE: should components be unregistered here?

    destroyed = true;
}

void Map::preprocess()
{
    // First, create a texture to hold the map
    const int mapWidth = _map->getWidth();
    const int mapHeight = _map->getHeight();
    const int tileWidth = _map->getTileWidth();
    const int tileHeight = _map->getTileHeight();
    auto texture = Graphics::Texture::create("PreprocessedMap",
                                             Vector2i(mapWidth * tileWidth,
                                                      mapHeight * tileHeight));

    // Iterate through map entities and blit their textures to the surface
    for (const GridRow &row : _entityGrid)
    {
        for (const EntityList &entities : row)
        {
            for (const std::shared_ptr<Entity> &entity : entities)
            {
                Entity::UUID uuid = entity->getUUID();
                std::shared_ptr<Components::Sprite> sprite =
                        getGame().getEntityMgr().getComponent<Components::Sprite>(uuid);
                std::shared_ptr<Components::Position> pos =
                        getGame().getEntityMgr().getComponent<Components::Position>(uuid);

                SDL_Rect destRect = { pos->x,
                                      mapHeight * tileHeight - pos->y,
                                      tileWidth,
                                      tileHeight };

                getGame().getRenderer().blitToTexture(sprite->texture,
                                                      nullptr,
                                                      texture->getName(),
                                                      &destRect);
            }
        }
    }

    // Replace the old preprocessed map texture with the new one
    if (_preprocessedTexture)
    {
        getGame().getTexMgr().remove(_preprocessedTexture->getName());
    }
    _preprocessedTexture.reset();
    _preprocessedTexture = texture;
    getGame().getTexMgr().add(texture->getName(),
                          texture);
}

Map::EntityList Map::getEntitiesAt(int col, int row)
{
    try
    {
        return _entityGrid.at(col).at(row);
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

    auto component = event.component;
    int col = component->x;
    int row = component->y;
    auto entity = component->getParent();

    try
    {
        _entityGrid.at(col).at(row).push_back(entity);
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
    //auto rgbStr = _map->getBackgroundColor().substr(1);
    //const SDL_Color colorKey = Color::parseRGBHexString(rgbStr);

    for (auto tileset : _map->getTileSets())
    {
        auto image = tileset->getImage();
        auto tilesetSurface = IMG_Load(image->getSource().string().c_str());
        if (!tilesetSurface)
        {
            throw Exceptions::MapException(IMG_GetError());
        }

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

        auto gid = tileset->getFirstGID();
        SDL_Rect srcRect;
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                std::ostringstream name;
                name << tileset->getName() << "-" << gid++;

                srcRect.x = (margin * (col + 1)) + (tileWidth * col);
                srcRect.y = (margin * (row + 1)) + (tileHeight * row);
                srcRect.w = tileWidth;
                srcRect.h = tileHeight;

                Graphics::Texture::create(name.str(),
                                          tilesetSurface,
                                          &srcRect);
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

            auto texture = getGame().getTexMgr().get(textureName.str());
            //entity->graphics =
            //        std::make_shared<Components::StaticSprite>(texture,
            //                                                   entity->position);
            Components::Sprite::create(entity, textureName.str());
//#define _DEBUG_ENTITIES
#if defined(_DEBUG_MAP) && defined(_DEBUG_ENTITIES)
            LOG_DEBUG << "created entity: (" << mapPos->x << "," << mapPos->y << ") -> "
                      << "(" << absPos->x << "," << absPos->y << ")";
#endif
        }

        cellID++;
    }
}
