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

#ifndef __SYSTEMS_GRAPHICS_HPP__
#define __SYSTEMS_GRAPHICS_HPP__

#include "defines.hpp"
#include "systems/base.hpp"

#include <map>

#include "components/sprite.hpp"
#include "events/subscriber.hpp"

namespace Systems
{
    class Graphics : public Base, public Events::Subscriber
    {
    public:
        Graphics();
        ~Graphics();

        void destroy();

        void update(float dt);

        void onEvent(const Events::SpriteComponentCreated &event);

        std::string toString() const;

    private:
        typedef ComponentList<Components::Sprite> SpriteComponentList;
        SpriteComponentList _spriteComponents;
    };
}

#endif
