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

#include "exception.hpp"
#include <cassert>
#include <sstream>
#include <tmxparser/Tmx.h>

Map::Map(const std::string &filename)
{
    // First, try to load the .tmx file
    _map = new Tmx::Map();
    _map->ParseFile(filename);

    if (_map->HasError())
    {
        std::ostringstream ss;
        ss << "Error " << static_cast<unsigned>(_map->GetErrorCode())
           << ": " << _map->GetErrorText();
        delete _map;
        throw MapException(ss.str());
    }

    // Try to load the associated tileset textures
}

Map::~Map()
{
    assert(_map != nullptr);
    delete _map;
}
