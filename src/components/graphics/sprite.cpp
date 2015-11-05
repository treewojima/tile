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

#ifdef _USE_NEW_COMPONENTS

#include "components/graphics/sprite.hpp"
#include <sstream>
#include "entity.hpp"
#include "events/dispatcher.hpp"

Components::Graphics::Sprite::Sprite(std::shared_ptr<Entity> parent,
                                     const TextureManager::Key &texture_,
                                     const std::string &debugName) :
    Components::Graphics::Base(parent, parent->getDebugName() + "Sprite" + debugName),
    texture(texture_)
{
}

std::string Components::Graphics::Sprite::toString() const
{
    std::ostringstream ss;
    ss << "Components::Graphics::Sprite[parent = " << getParent() << ", "
       << "debugName = \"" << getDebugName() << "\", "
       << "texture = \"" << texture << "\"]";
    return ss.str();
}

std::shared_ptr<Components::Graphics::Sprite>
Components::Graphics::Sprite::create(std::shared_ptr<Entity> parent,
                                     const TextureManager::Key &texture_,
                                     const std::string &debugName)
{
    auto ptr = std::shared_ptr<Sprite>(new Sprite(parent, texture_, debugName));
    Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
    Events::Dispatcher::raise<Events::SpecificComponentCreated<Sprite>>(ptr);
    return ptr;
}

#endif
