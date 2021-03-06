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

#ifndef _USE_NEW_COMPONENTS

#include "components/staticsprite.hpp"
#include "graphics.hpp"

Components::StaticSprite::StaticSprite(
        std::shared_ptr<Texture> texture,
        std::shared_ptr<Components::Position> positionComponent) :
    Components::Graphics("graphics"),
    _position(positionComponent)
{
    setTexture(texture);
}

void Components::StaticSprite::draw()
{
    if (getTexture().get() != nullptr && _position.get() != nullptr)
    {
        ::Graphics::blitTexture(getTexture(), _position->x, _position->y);
    }
}

std::string Components::StaticSprite::toString() const
{
    std::ostringstream ss;
    ss << "Components::StaticSprite[name = \"" << getName()
       << "\", texture = "
       << (_texture.get() != nullptr ? _texture->toString() : "<null>")
       << ", positionComponent = "
       << (_position.get() != nullptr ? _position->toString() : "<null>")
       << "]";
    return ss.str();
}

void Components::StaticSprite::setTexture(std::shared_ptr<Texture> texture)
{
    _texture = texture;
}

#endif
