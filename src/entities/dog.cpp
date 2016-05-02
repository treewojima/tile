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

#ifdef _USE_NEW_ENTITY

#include "entities/dog.hpp"

#include <memory>

#include "components/eventsubscriber.hpp"
#include "components/position.hpp"
#include "color.hpp"
#include "events/dispatcher.hpp"
#include "game.hpp"
#include "helper_events.hpp"
#include "window.hpp"

namespace
{
    class DogEventSubscriber : public Events::Subscriber
    {
    public:
        DogEventSubscriber(std::shared_ptr<Entity> entity)
        {
            _pos = Game::getEntityMgr().getComponent<Components::Position>(entity->getUUID());
            _mapPos = Game::getEntityMgr().getComponent<Components::MapPosition>(entity->getUUID());
        }

        void onEvent(const Events::KeyDown &e)
        {
            if (e.keys[SDL_SCANCODE_LEFT])
            {
                _pos->x -= MAGNITUDE;
            }

            if (e.keys[SDL_SCANCODE_RIGHT])
            {
                _pos->x += MAGNITUDE;
            }

            if (e.keys[SDL_SCANCODE_DOWN])
            {
                _pos->y -= MAGNITUDE;
            }

            if (e.keys[SDL_SCANCODE_UP])
            {
                _pos->y += MAGNITUDE;
            }
        }

        void onEvent(const Events::MouseDown &e)
        {
            if (e.button != Events::MouseDown::Button::Right) return;

            auto col = e.position.x / 32;
            auto row = (Window::getHeight() - e.position.y) / 32;

            Actions::Movement m(_mapPos->getParent(),
                                _mapPos->toVector(),
                                Vector2i(col, row));
            Game::getMovementSys().queueMovement(std::move(m));
        }

        std::string toString() const
        {
            return "DogEventSubscriber";
        }

    private:
        static const int MAGNITUDE = 1;

        std::shared_ptr<Components::Position> _pos;
        std::shared_ptr<Components::MapPosition> _mapPos;
    };
}

std::shared_ptr<Entity> createDog()
{
    auto dog = Entity::create("Dog");

    SDL_Color colorKey = Color::makeColor(255, 0, 255);
    auto texture = std::make_shared<Texture>("Dog", "res/dog/dog_down_0.png", &colorKey);
    Game::getTexMgr().add(texture->getName(), texture);

    //Game::getGraphicsSys().createSpriteComponent(dog, texture->getName());
    auto mapPos = Components::MapPosition::create(dog, 2, 2);
    auto pos = Components::Position::create(dog, *mapPos);

    /*Components::Position::create(dog,
								 Window::getWidth() / 2,
							     Window::getHeight() / 2);
    Components::MapPosition::create(dog,
                                    Window::getWidth() / 64,
                                    Window::getHeight() / 64);*/

    Components::Graphics::Sprite::create(dog,
                                         texture->getName(),
                                         "Down");

    auto subscriber = std::make_shared<DogEventSubscriber>(dog);
    Events::Dispatcher::subscribe<Events::KeyDown>(*subscriber);
    Events::Dispatcher::subscribe<Events::MouseDown>(*subscriber);
    Components::EventSubscriber::create(dog, subscriber);

    return dog;
}

#else

#include "entities/dog.hpp"

#include <sstream>

#include "colors.hpp"
#include "logger.hpp"
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

	auto startX = Window::getWidth() / 4;
	auto startY = Window::getHeight() - (Window::getHeight() / 4);
	position = std::make_shared<Components::Position>(startX, startY);
	
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
	graphics = _directions[Direction::Down];

	(*properties)["string"] = std::string("hai2u");
	(*properties)["bool"] = true;
	(*properties)["int"] = 17;
}

Dog::~Dog()
{
	for (auto handle : _eventHandles)
	{
		Game::unregisterEvent(handle);
	}

	LOG_DEBUG << *properties;
}

void Dog::loadTextures()
{
	if (_texturesLoaded) return;

	const SDL_Color colorKey = Color::makeColor(255, 0, 255);
	std::shared_ptr<Texture> texture;

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

#endif
