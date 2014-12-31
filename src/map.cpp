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
#include <sstream>
#include <tmxparser/Tmx.h>

#include "colors.hpp"
#include "exception.hpp"
#include "game.hpp"

namespace
{
    // NOTE: these should really be moved to a more appropriate location
    SDL_Surface *createBlankSDLSurface(Vector2i dimensions,
                                       SDL_Color color = Colors::WHITE);
    TextureManager::ResourcePtr createBlankTexture(const std::string &name,
                                                   Vector2i dimensions,
                                                   SDL_Color color = Colors::WHITE);
}

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
    render();
}

Map::~Map()
{
    assert(glIsFramebufferEXT(_fbo));
    glDeleteFramebuffersEXT(1, &_fbo);

    assert(_map != nullptr);
    delete _map;
}

void Map::loadTilesetTextures()
{
    auto tilesets = _map->GetTilesets();
    _tilesetTextures.resize(tilesets.size());

    for (auto tileset : tilesets)
    {
        TextureManager::ResourcePtr texture;

        try
        {
            auto filename = "res/" + tileset->GetImage()->GetSource();
            texture = std::make_shared<Texture>(tileset->GetName(),
                                                filename);
        }
        catch (const SDLException &e)
        {
            std::ostringstream ss;
            ss << "Couldn't load tileset texture for map \"" << getName()
               << "\": " << e.what();
            throw MapException(ss.str());
        }

        Game::getTexMgr().add(texture->getName(), texture);
        _tilesetTextures.push_back(texture);
    }
}

void Map::render()
{
    // First, try to create a framebuffer object
    glGenFramebuffersEXT(1, &_fbo);
    auto error = glGetError();
    if (error != GL_NO_ERROR)
        throw GLException(error);

    // Create a blank surface
    const int widthPixels = _map->GetWidth() * _map->GetTileWidth();
    const int heightPixels = _map->GetHeight() * _map->GetTileHeight();
    auto texture = createBlankTexture("renderedMap", Vector2i(widthPixels, heightPixels));

    auto layers = _map->GetLayers();
    for (Tmx::Layer *layer : layers)
    {
        for (int x = 0; x < layer->GetWidth(); x++)
        {
            for (int y = 0; y < layer->GetHeight(); y++)
            {

            }
        }
    }
}

// Local functions
namespace {

SDL_Surface *createBlankSDLSurface(Vector2i dimensions, SDL_Color color)
{
    auto surface = SDL_CreateRGBSurface(0,
                                        dimensions.x,
                                        dimensions.y,
                                        32,
                                        Colors::RMASK,
                                        Colors::GMASK,
                                        Colors::BMASK,
                                        Colors::AMASK);
    if (surface == nullptr)
        throw SDLException();

    SDL_FillRect(surface,
                 nullptr,
                 SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));

    return surface;
}

TextureManager::ResourcePtr createBlankTexture(const std::string &name,
                                               Vector2i dimensions,
                                               SDL_Color color)
{
    auto surface = createBlankSDLSurface(dimensions, color);
    return std::make_shared<Texture>(name, surface);
}

}
