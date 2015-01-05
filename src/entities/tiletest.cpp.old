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

#include "defines.hpp"
#include "entities/tiletest.hpp"

#include <iostream>

#include "game.hpp"
#include "graphics.hpp"

TileTest::TileTest(const std::string &name,
                   const Vector2f &pos,
                   TextureManager::ResourcePtr texture) :
    Entity(name),
    _position(pos),
    _texture(texture)
{
#ifdef _DEBUG_ENTITIES
    std::cout << "created entity " << toString() << std::endl;
#endif
}

void TileTest::draw() const
{
    Graphics::blitTexture(_texture, _position);
}

std::string TileTest::toString() const
{
    std::ostringstream ss;
    ss << "TileTest[name = \"" << getName() << "\", position = " << _position << "]";
    return ss.str();
}
