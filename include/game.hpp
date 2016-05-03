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

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "defines.hpp"

#include <memory>

#include "camera.hpp"
#include "entitymanager.hpp"
#include "graphics/renderer.hpp"
//#include "map.hpp"
#include "helper_events.hpp"
#include "statemanager.hpp"
#include "systems/graphics.hpp"
#include "systems/movement.hpp"
#include "timer.hpp"

class Game : public Events::Subscriber
{
public:
    struct Options
    {
		std::string programName;
        unsigned windowWidth, windowHeight;
		bool vsync;
		std::string logFile;
    };

    Game(const Options &options);

    void run();
    void exit(const std::exception *e = nullptr);

    void onEvent(const Events::MouseDown &event);
    void onEvent(const Events::Quit &event);

    inline bool isRunning() { return _running; }
    inline void setRunning(bool b) { _running = b; }

    inline const Options &getOptions() { return _options; }

    inline Graphics::Window &getWindow() { return *_window; }
    inline Graphics::Renderer &getRenderer() { return *_renderer; }
    inline Camera &getCamera() { return *_camera; }

    inline EntityManager &getEntityMgr() { return *_entityMgr; }
    inline StateManager &getStateMgr() { return *_stateMgr; }
    inline Graphics::TextureManager &getTexMgr() { return *_texMgr; }

    inline Systems::Graphics &getGraphicsSys() { return *_graphicsSys; }
    Systems::Movement &getMovementSys() { return *_movementSys; }

    std::string toString() const { return "Game[]"; }

private:
    // Subsystem initialization/shutdown
    void initSDL();
#ifdef _ENABLE_AUDIO
    initAudio();
#endif
    void shutdownSDL();

    // Event handling
    void registerEvents();
    void handleEvents();
    void leftClick(int col, int row);
    void rightClick(int col, int row);

    // Game options and internal state
    Options _options;
    bool _running;

    // Renderer and related objects
    std::unique_ptr<Graphics::Renderer> _renderer;
    std::shared_ptr<Graphics::Window> _window;
    std::unique_ptr<Camera> _camera;

    // FPS timer
    std::unique_ptr<Timer> _fpsTimer;

    // Test map
    //std::unique_ptr<Map> _map;

    // Entity and component systems
    std::unique_ptr<EntityManager> _entityMgr;
    std::unique_ptr<StateManager> _stateMgr;
    std::unique_ptr<Graphics::TextureManager> _texMgr;
    std::unique_ptr<Systems::Graphics> _graphicsSys;
    std::unique_ptr<Systems::Movement> _movementSys;
};

// Global game instance getter (in main.cpp)
Game &getGame();

#endif
