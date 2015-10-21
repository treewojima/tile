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

#ifndef __DOG_HPP__
#define __DOG_HPP__

#include "defines.hpp"

#include <memory>
#include <vector>

#include "components/animatedsprite.hpp"
#include "entity.hpp"
#include "game.hpp"

class Dog : public Entity
{
public:
	Dog();
	~Dog();

private:
	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
		NUM_DIRECTIONS
	};

	static bool _texturesLoaded;
	static const int NUM_FRAMES;
	static const char *_strings[];
	
	std::shared_ptr<Components::AnimatedSprite> _directions[NUM_DIRECTIONS];
	std::vector<Game::Event::Handle> _eventHandles;

	static void loadTextures();

	void registerEvents();
};

#endif

