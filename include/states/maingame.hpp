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

#ifndef __STATES_MAINGAME_HPP__
#define __STATES_MAINGAME_HPP__

#include "defines.hpp"

#include <list>
#include <memory>

#include "entity.hpp"
#include "map/map.hpp"
#include "states/base.hpp"

namespace States
{
    class MainGame : public Base
    {
    public:
        MainGame() : Base(Type::Main) {}

        void initialize();
        void destroy();

        void draw(float dt);

    private:
        std::shared_ptr<Map::Map> _map;

        void loadTextures();
        void createEntities();
    };
}

#endif
