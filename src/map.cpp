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
//#include <tmx/LayerVisitor.h>
#include <tmx/TileLayer.h>

#include "colors.hpp"
#include "components/position.hpp"
#include "components/staticsprite.hpp"
//#include "entities/tiletest.hpp"
#include "entity.hpp"
#include "exception.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "window.hpp"

Map::Map(const std::string &name, const std::string &filename) :
    _name(name)
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
    _entities.clear();

    //assert(_map != nullptr);
    //delete _map;
}

void Map::draw() const
{
    for (auto entity : _entities)
    {
        entity->graphics->draw();
    }
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
        const int tileHeight = tileset->getTileHeight();
        const int tilesetWidth = tilesetSurface->w;
        const int tilesetHeight = tilesetSurface->h;
        const int numCols = tilesetWidth / tileWidth;
        const int numRows = tilesetHeight / tileHeight;
        const int margin = tileset->getMargin();

        std::cout << "tileWidth     = "   << tileWidth
                  << "\ntileHeight    = " << tileHeight
                  << "\ntilesetWidth  = " << tilesetWidth
                  << "\ntilesetHeight = " << tilesetHeight
                  << "\nmargin        = " << margin
                  << "\nnumCols       = " << numCols
                  << "\nnumRows       = " << numRows
                  << "\nfirstGid      = " << tileset->getFirstGID() << std::endl;

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
            auto row = cellID / map.getWidth();
            assert(row < map.getHeight());
            float x = col * map.getTileWidth();
            float y = row * map.getTileHeight();

            // Create the ***TEST*** entity
            std::ostringstream entityName;
            entityName << layerName << "-" << col << "-" << row
                       << "-" << textureName.str();
            auto entity = std::make_shared<Entity>(entityName.str());

            Vector2f pos(x + 50, Window::getHeight() - (y + 50));
            entity->position =
                    std::make_shared<Components::Position>(pos);

            auto texture = Game::getTexMgr().get(textureName.str());
            entity->graphics =
                    std::make_shared<Components::StaticSprite>(texture,
                                                               entity->position);

			

            _parent->_entities.push_back(entity);
        }

        cellID++;
    }
}
