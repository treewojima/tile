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
#include "systems/graphics.hpp"

#include "components/position.hpp"
#include "entity.hpp"
#include "events/dispatcher.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "logger.hpp"
#include "window.hpp"

Systems::Graphics::Graphics() : Base()
{
    Events::Dispatcher::subscribe<Events::SpriteComponentCreated>(*this);
}

Systems::Graphics::~Graphics()
{
    if (!_destroyed) destroy();
}

void Systems::Graphics::destroy()
{
	if (_destroyed) return;

    Events::Dispatcher::unsubscribe(*this);
    _spriteComponents.clear();

    _destroyed = true;
}

void Systems::Graphics::draw()
{
    Window::clear(255, 255, 255);

    // NOTE: This is pretty naive and slow. Make sure to implement some sort of caching
    //       system to optimize this!

    for (auto &sprite : _spriteComponents)
    {
        const auto &parent = sprite->getParent();
        const auto &pos = Game::getEntityMgr().getComponent<Components::Position>(parent->getUUID());
        if (pos)
        {
            ::Graphics::blitTexture(sprite->texture, pos->x, pos->y);
        }
        else
        {
			std::ostringstream ss;
			ss << "no matching position component for " << sprite;
			throw Exception(ss.str());
        }
    }

    Window::flip();
}

void Systems::Graphics::onEvent(const Events::SpriteComponentCreated &event)
{
    auto component = event.component;

    _spriteComponents.push_back(component);
}

std::string Systems::Graphics::toString() const
{
	std::ostringstream ss;
	ss << "Systems::Graphics[spriteComponentCount = " << _spriteComponents.size() << "]";
	return ss.str();
}
