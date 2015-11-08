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
#include "components/mapposition.hpp"

#include "entity.hpp"
#include "events/dispatcher.hpp"

Components::MapPosition::MapPosition(std::shared_ptr<Entity> parent,
									 int x_,
						             int y_) :
	Base(parent, parent->getDebugName() + "Position"),
	x(x_),
	y(y_)
{
}

std::string Components::MapPosition::toString() const
{
	std::ostringstream ss;
	ss << "Components::MapPosition[debugName = \"" << getDebugName() << "\", "
		<< "parent = " << getParent() << "\", "
		<< "position = " << toVector() << "]";
	return ss.str();
}

std::shared_ptr<Components::MapPosition>
Components::MapPosition::create(std::shared_ptr<Entity> parent,
								int x,
								int y)
{
	auto ptr = std::shared_ptr<MapPosition>(new MapPosition(parent, x, y));
	Events::Dispatcher::raise<Events::ComponentCreated>(ptr);
	Events::Dispatcher::raise<Events::MapPositionComponentCreated>(ptr);
	return ptr;
}

std::shared_ptr<Components::MapPosition>
Components::MapPosition::create(std::shared_ptr<Entity> parent,
								const Vector2i &v)
{
	return create(parent, v.x, v.y);
}
