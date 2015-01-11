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
#include <tmxparser/Tmx.h>

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
    // First, try to load the .tmx file
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

    loadTilesetTextures();
    createTestEntities();
}

Map::~Map()
{
    _entities.clear();

    assert(_map != nullptr);
    delete _map;
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
    // NOTE: This is likely very, very wrong for multiple tilesets
    int gid = 0;

    // NOTE: This is very, very hacky
    const SDL_Color colorKey = Colors::makeColor(0xFF, 0, 0xFF);

    const int numTilesets = _map->GetNumTilesets();
    for (int i = 0; i < numTilesets; i++)
    {
        const Tmx::Tileset *tileset = _map->GetTileset(i);

        const int tileWidth = tileset->GetTileWidth();
        const int tileHeight = tileset->GetTileHeight();
        const int tilesetWidth = tileset->GetImage()->GetWidth();
        const int tilesetHeight = tileset->GetImage()->GetHeight();
        const int margin = tileset->GetMargin();
        const int numCols = tilesetWidth / tileWidth;
        const int numRows = tilesetHeight / tileHeight;
        const int firstGid = tileset->GetFirstGid();

        std::cout << "tileWidth     = "   << tileWidth
                  << "\ntileHeight    = " << tileHeight
                  << "\ntilesetWidth  = " << tilesetWidth
                  << "\ntilesetHeight = " << tilesetHeight
                  << "\nmargin        = " << margin
                  << "\nnumCols       = " << numCols
                  << "\nnumRows       = " << numRows
                  << "\nfirstGid      = " << firstGid << std::endl;

        auto filename = tileset->GetImage()->GetSource();
        auto tilesetSurface = Graphics::loadSDLSurface("res/" + filename);

        // NOTE: This is likely very, very wrong for multiple tilesets
        //gid = firstGid;

        for (int row = 0; row < numRows; row++)
        {            
            for (int col = 0; col < numCols; col++)
            {
                SDL_Rect srcRect;
                srcRect.x = (margin * (col + 1)) + (tileWidth * col);
                srcRect.y = (margin * (row + 1)) + (tileHeight * row);
                srcRect.w = tileWidth;
                srcRect.h = tileHeight;

                //std::cout << "srcRect[" << col << ", " << row << "] = {"
                //          << srcRect.x << ", " << srcRect.y << ", "
                //          << srcRect.w << ", " << srcRect.h << "}" << std::endl;

                auto destSurface = Graphics::createBlankSDLSurface(tileWidth, tileHeight);
                assert(destSurface != nullptr);

                SDL_BlitSurface(tilesetSurface,
                                &srcRect,
                                destSurface,
                                nullptr);

                std::ostringstream name;
                //name << tileset->GetName() << "-" << col << "-" << row;
                name << tileset->GetName() << "-" << gid;

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

                // Create a test entity
                //Vector2f pos(srcRect.x + 50 + (margin + col * 2),
                //             srcRect.y + 50 + (margin + row * 2));
                //auto entity = std::make_shared<TileTest>(texture->getName(),
                //                                         pos);
                //_entities.push_back(entity);

                gid++;
            }
        }

        SDL_FreeSurface(tilesetSurface);
    }
}

void Map::createTestEntities()
{
    const int numLayers = _map->GetNumLayers();
    for (int i = 0; i < numLayers; i++)
    {
        //if (i > 0) break;

        const Tmx::Layer *layer = _map->GetLayer(i);

        //std::cout << "**************\n"
        //          << "Layer " << layer->GetName()
        //          << "\n**************" << std::endl;

        for (int col = 0; col < layer->GetWidth(); col++)
        {
            for (int row = 0; row < layer->GetHeight(); row++)
            {
                auto tile = layer->GetTile(col, row);                

                //if (tile.id == 0) continue;

                // HACK ALERT HACK ALERT HACK ALERT
                std::ostringstream name, textureName;
                name << "desert-" << col << "-" << row;
                textureName << "desert-" << tile.id;

                auto texture = Game::getTexMgr().get(textureName.str());

                // Create the test entity
                Vector2f pos(col * _map->GetTileWidth() + 50,
                             Window::getHeight() - (row * _map->GetTileHeight() + 50));
                auto entity = std::make_shared<Entity>(name.str());
                entity->position = std::make_shared<Components::Position>(pos);
                entity->graphics =
                        std::make_shared<Components::StaticSprite>(texture,
                                                                   entity->position);

                _entities.push_back(entity);
            }
        }
    }
}
