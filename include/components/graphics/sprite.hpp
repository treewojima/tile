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

#ifndef __COMPONENTS_GRAPHICS_SPRITE_HPP__
#define __COMPONENTS_GRAPHICS_SPRITE_HPP__

#include "defines.hpp"

#ifdef _USE_NEW_COMPONENTS

#include "components/graphics/base.hpp"
#include "texture.hpp"

namespace Components
{
    namespace Graphics
    {
        class Sprite final : public Base
        {
        private:
            Sprite(std::shared_ptr<Entity> parent,
                   const TextureManager::Key &texture_,
                   const std::string &debugName);

        public:
            TextureManager::Key texture;

            std::string toString() const;

            static std::shared_ptr<Sprite> create(std::shared_ptr<Entity> parent,
                                                  const TextureManager::Key &texture_,
                                                  const std::string &debugName = "");
        };
    }
}

namespace Events
{
    typedef SpecificComponentCreated<Components::Graphics::Sprite> SpriteComponentCreated;
}

#endif

#endif
