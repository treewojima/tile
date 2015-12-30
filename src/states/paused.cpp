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
#include "states/paused.hpp"

#include "logger.hpp"
#include "window.hpp"

void States::Paused::initialize()
{
	LOG_DEBUG << "initialized PausedState";
}

void States::Paused::destroy()
{
	LOG_DEBUG << "destroyed PausedState";
}

void States::Paused::draw(float dt)
{
	Window::clear(0, 0, 255);
	Window::flip();
}
