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

#include "entities/dog.hpp"

#include <sstream>

#include "colors.hpp"
#include "texture.hpp"
#include "window.hpp"

bool Dog::_texturesLoaded = false;
const int Dog::NUM_FRAMES = 4;
const char *Dog::_strings[Dog::Direction::NUM_DIRECTIONS] =
	{ "up", "down", "left", "right" };

Dog::Dog() :
	Entity("dog"),
	_eventHandles(Direction::NUM_DIRECTIONS)
{
	// NOTE: This doesn't account for multiple dogs!
	registerEvents();

	loadTextures();

	position = std::make_shared<Components::Position>(Window::getWidth() / 2,
		Window::getHeight() / 2);
	
	for (int dir = 0; dir < Direction::NUM_DIRECTIONS; dir++)
	{
		Components::AnimatedSprite::TextureList textures;
		for (int i = 0; i < NUM_FRAMES; i++)
		{
			std::ostringstream ss;
			ss << "dog_" << _strings[dir] << "_" << i;
			textures.push_back(Game::getTexMgr().get(ss.str()));
		}

		_directions[dir] = std::make_shared<Components::AnimatedSprite>(textures,
																		position);
	}
	graphics = _directions[Direction::Up];
}

Dog::~Dog()
{
	for (auto handle : _eventHandles)
	{
		Game::unregisterEvent(handle);
	}
}

void Dog::loadTextures()
{
	if (_texturesLoaded) return;

	const SDL_Color colorKey = Colors::makeColor(255, 0, 255);
	TextureManager::ResourcePtr texture;

	for (int dir = 0; dir < Direction::NUM_DIRECTIONS; dir++)
	{
		for (int i = 0; i < NUM_FRAMES; i++)
		{
			std::ostringstream ss;
			ss << "dog_" << _strings[dir] << "_" << i;
			texture = std::make_shared<Texture>(ss.str(),
				"res/dog/" + ss.str() + ".png",
				&colorKey);
			Game::getTexMgr().add(texture->getName(), texture);
		}
	}

	_texturesLoaded = true;
}
void Dog::registerEvents()
{
	_eventHandles[Direction::Up] = Game::registerEvent(
		SDL_SCANCODE_UP,
		[this](const SDL_Event &e) { graphics = _directions[Direction::Up]; },
		"DogUpEvent");

	_eventHandles[Direction::Down] = Game::registerEvent(
		SDL_SCANCODE_DOWN,
		[this](const SDL_Event &e) { graphics = _directions[Direction::Down]; },
		"DogDownEvent");

	_eventHandles[Direction::Left] = Game::registerEvent(
		SDL_SCANCODE_LEFT,
		[this](const SDL_Event &e) { graphics = _directions[Direction::Left]; },
		"DogLeftEvent");

	_eventHandles[Direction::Right] = Game::registerEvent(
		SDL_SCANCODE_RIGHT,
		[this](const SDL_Event &e) { graphics = _directions[Direction::Right]; },
		"DogRightEvent");
}
