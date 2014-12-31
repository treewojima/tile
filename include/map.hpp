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

#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "defines.hpp"

#include <string>
#include <tmxparser/TmxMap.h>
#include <vector>

#include "texture.hpp"

class Map
{
public:
    Map(const std::string &name, const std::string &filename);
    ~Map();

    inline std::string getName() const { return _name; }

private:
    std::string _name;
    Tmx::Map *_map;
    std::vector<TextureManager::ResourcePtr> _tilesetTextures;
    TextureManager::ResourcePtr _renderedMapTexture;
    GLuint _fbo;

    void loadTilesetTextures();

    // NOTE: This is NOT the best way to go about this
    void render();
};

#endif
