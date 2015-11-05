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

#ifndef __COMPONENTS_ANIMATEDSPRITE_HPP__
#define __COMPONENTS_ANIMATEDSPRITE_HPP__

#include "defines.hpp"

#ifndef _USE_NEW_COMPONENTS

#include <list>
#include "components/graphics.hpp"
#include "components/position.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "vector.hpp"

namespace Components
{
    class AnimatedSprite : public Graphics
    {
    public:
        typedef std::list<std::shared_ptr<Texture>> TextureList;

        AnimatedSprite(TextureList textures,
                       std::shared_ptr<Components::Position> positionComponent);

        void draw();

        std::string toString() const;

    private:
        TextureList _textures;
        TextureList::iterator _currentTextureIter;
        std::shared_ptr<Components::Position> _position;
        Timer _timer;
    };
}

#endif

#endif

