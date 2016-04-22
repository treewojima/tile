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
#include "states/maingame.hpp"

#include <sstream>

#include "color.hpp"
#include "entities/dog.hpp"
#include "game.hpp"
#include "logger.hpp"
#include "systems/graphics.hpp"

void States::MainGame::initialize()
{
    LOG_DEBUG << "initialized States::MainGame";

    loadTextures();
    createEntities();

    //_map = std::make_unique<Map>("desert", "res/desert.tmx");
}

void States::MainGame::destroy()
{
    LOG_DEBUG << "destroyed MainGameState";
}

void States::MainGame::postLoop()
{
}

void States::MainGame::update(float dt)
{
}

void States::MainGame::draw(float dt)
{
    Game::getGraphicsSys().update(dt);
}

void States::MainGame::loadTextures()
{
    const SDL_Color colorKey = Color::makeColor(255, 0, 255);
    std::shared_ptr<Texture> texture;

    //texture = std::make_shared<Texture>("tux", "res/tux.png", &colorKey);
    //_texMgr.add(texture->getName(), texture);

    //texture = std::make_shared<Texture>("beastie", "res/beastie.png", &colorKey);
    //_texMgr.add(texture->getName(), texture);

    for (int i = 1; i <= 4; i++)
    {
        std::ostringstream ss;
        ss << "foo" << i;
        texture = std::make_shared<Texture>(ss.str(), "res/" + ss.str() + ".png", &colorKey);
        Game::getTexMgr().add(texture->getName(), texture);
    }
}

void States::MainGame::createEntities()
{
#if 0
    //_entities.push_back(std::make_shared<Tux>(Vector2f::ZERO));

    auto tux = std::make_shared<Tux>();
    tux->position = std::make_shared<Components::Position>(Window::getWidth() / 2,
        Window::getHeight() / 2);

    Components::AnimatedSprite::TextureList textures;
    //textures.push_back(_texMgr.get("tux"));
    //textures.push_back(_texMgr.get("beastie"));
    for (int i = 1; i <= 4; i++)
    {
        std::ostringstream ss;
        ss << "foo" << i;
        textures.push_back(Game::getTexMgr().get(ss.str()));
    }

    tux->graphics = std::make_shared<Components::AnimatedSprite>(textures,
        tux->position);

    _entities.push_back(tux);
#endif

    //_entities.push_back(std::make_shared<Dog>());
    createDog();
}

