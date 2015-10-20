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

#ifndef __MAINGAMESTATE_HPP__
#define __MAINGAMESTATE_HPP__

#include "defines.hpp"

#include <list>
#include <memory>

#include "entity.hpp"
#include "map.hpp"
#include "state.hpp"

class MainGameState : public State
{
public:
	MainGameState() : State(Type::Main) {}

	void initialize();
	void destroy();

	void postLoop();

	void update(float dt);
	void draw();

private:
	std::list<std::shared_ptr<Entity>> _entities;
	std::unique_ptr<Map> _map;

	void loadTextures();
	void createEntities();
};

#endif
