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

#include "color.hpp"
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

#ifdef PREPROCESS_MAP
    // Create the preprocessed map texture
    preprocess();
#endif
}

Map::~Map()
{
    if (!_destroyed) destroy();
}

void Map::destroy()
{
	if (_destroyed) return;

    Events::Dispatcher::unsubscribe(*this);

    _entityGrid.clear();
#ifdef PREPROCESS_MAP
    Game::getTexMgr().remove(_preprocessedTexture->getName());
    _preprocessedTexture.reset();
#endif

    // NOTE: should components be unregistered here?

	_destroyed = true;
}

#ifdef PREPROCESS_MAP
void Map::preprocess()
{
    // First, create an SDL surface to hold the map
    const int mapWidth = _map->getWidth();
    const int mapHeight = _map->getHeight();
    const int tileWidth = _map->getTileWidth();
    const int tileHeight = _map->getTileHeight();
    SDL_Surface *surface = Graphics::createBlankSDLSurface(mapWidth * tileWidth,
                                                           mapHeight * tileHeight);

    // Iterate through map entities and blit their textures to the surface
    for (const GridRow &row : _entityGrid)
    {
        for (const EntityList &entities : row)
        {
            for (const std::shared_ptr<Entity> &entity : entities)
            {
                Entity::UUID uuid = entity->getUUID();
                std::shared_ptr<Components::Graphics::Sprite> sprite =
                        Game::getEntityMgr().getComponent<Components::Graphics::Sprite>(uuid);
                std::shared_ptr<Components::Position> pos =
                        Game::getEntityMgr().getComponent<Components::Position>(uuid);

                TextureManager::Resource texture = Game::getTexMgr()[sprite->texture];
                SDL_Rect destRect = { pos->x,
                                      mapHeight * tileHeight - pos->y,
                                      tileWidth,
                                      tileHeight };
                SDL_BlitSurface(texture->getSurface(),
                                nullptr,
                                surface,
                                &destRect);
            }
        }
    }

    // Replace the old preprocessed map texture with the new one
    if (_preprocessedTexture)
    {
        Game::getTexMgr().remove(_preprocessedTexture->getName());
    }
    _preprocessedTexture.reset();
    auto rgbStr = _map->getBackgroundColor().substr(1);
    const SDL_Color colorKey = Color::parseRGBHexString(rgbStr);
    _preprocessedTexture = std::make_shared<Texture>("PreprocessedMap",
                                                     surface);
    Game::getTexMgr().add(_preprocessedTexture->getName(),
                          _preprocessedTexture);
}
#endif

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
                                                         &Color::COLOR_KEY, //&colorKey,
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
