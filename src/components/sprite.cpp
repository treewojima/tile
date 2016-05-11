/* game
 * Copyright (C) 2014-2016 Scott Bishop <treewojima@gmail.com>
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

#include "components/sprite.hpp"
#include <sstream>
#include "events/dispatcher.hpp"

std::shared_ptr<Components::Sprite>
Components::Sprite::create(const Entity::UUID &parent,
                           const Graphics::TextureManager::Key &texture_,
                           const std::string &debugName)
{
    auto ptr = std::shared_ptr<Sprite>(new Sprite(parent, texture_, debugName));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::SpecificComponentCreated<Sprite>>(ptr);
    return ptr;
}

Components::Sprite::Sprite(const Entity::UUID &parent,
                           const Graphics::TextureManager::Key &texture_,
                           const std::string &debugName) :
    Components::Base(parent, debugName),
    texture(texture_)
{
}

std::string Components::Sprite::toString() const
{
    std::ostringstream ss;
    ss << "Components::Sprite[parent = " << getParent() << ", "
       << "debugName = \"" << getDebugName() << "\", "
       << "texture = \"" << texture << "\"]";
    return ss.str();
}
