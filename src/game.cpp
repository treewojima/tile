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
#include "game.hpp"

#include <algorithm>
#include <Box2D/Box2D.h>
#include <easylogging++.h>
#include <GL/glew.h>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <tclap/CmdLine.h>

#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/cursor.hpp"
#include "entities/paddle.hpp"
#include "entities/walls.hpp"
#include "exception.hpp"
#include "helper_events.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "timer.hpp"
#include "window.hpp"

Uint32 Game::Event::CUSTOM_KEYPRESS_EVENT;

// Locals
namespace
{
    bool _running = false;
    std::map<Game::Event::Handle, std::shared_ptr<Game::Event>> _events;
    Timer _fpsTimer;
    std::unique_ptr<Camera> _camera;

    std::list<std::shared_ptr<Entity>> _entities;
    std::shared_ptr<Level> _level;

    void initSDL();
    void initGL();
    void shutdownSDL();
    void initEntities();
    void destroyEntities();
    void registerEvents();
    void handleEvents();
    void updateEntities(float dt);
    void cullDeadEntities();
    void drawScene();
}

void Game::run(Game::Options options)
{
    try
    {
        initSDL();
        Window::create(options.windowWidth, options.windowHeight);

        auto screenRatio = static_cast<float>(options.windowWidth) /
                           static_cast<float>(options.windowHeight);
        _camera = std::unique_ptr<Camera>(
            new Camera(b2Vec2(0, 0),                                      // worldMin
                       b2Vec2(10 * screenRatio, 10),                      // worldMax
                       b2Vec2(0, 0),                                      // screenMin
                       b2Vec2(options.windowWidth, options.windowHeight), // screenMax
                       b2Vec2(0, 0),                                      // screenMinInWorld
                       b2Vec2(10 * screenRatio, 10)));                    // screenMaxInWorld
        //LOG(DEBUG) << *_camera;

        initGL();
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << "EXCEPTION: " << e.what();
    }

    Physics::initialize();
    registerEvents();
    initEntities();

    Timer stepTimer;
    float dt = 0;

    int countedFrames = 0;
    Uint32 oldTicks = 0, currentTicks = 0;

    setRunning(true);
    _fpsTimer.start();
    stepTimer.start();
    while (isRunning())
    {
        // Update the FPS counter every second or so
        currentTicks = _fpsTimer.getTicks();
        if (currentTicks - oldTicks >= 1000)
        {
            float avgFPS = countedFrames / (currentTicks / 1000.f);
            std::ostringstream ss;
            ss << "FPS: " << avgFPS;
            Window::setTitle(ss.str());
            oldTicks = currentTicks;
        }

        try
        {
            handleEvents();

            dt = stepTimer.getTicks() / 1000.f;
            Physics::step(dt);
            updateEntities(dt);
            stepTimer.start();

            drawScene();

            cullDeadEntities();
        }
        catch (std::exception &e)
        {
            LOG(ERROR) << "EXCEPTION: " << e.what();
            setRunning(false);
        }

        countedFrames++;
    }
    _fpsTimer.stop();

    Window::destroy();
    destroyEntities();
    shutdownSDL();
}

bool Game::isRunning()
{
    return _running;
}

void Game::setRunning(bool b)
{
    _running = b;
}

Camera &Game::getCamera()
{
    return *_camera;
}

Game::Event::Handle Game::registerEvent(SDL_Scancode key,
                                        Game::Event::Callback callback,
                                        const std::string &debugString)
{
    return registerEvent(std::shared_ptr<KeyEvent>(
                             new KeyEvent(key, callback, debugString)));
}

Game::Event::Handle Game::registerEvent(std::shared_ptr<Game::Event> event)
{
    static Event::Handle currentHandle = 0;

    _events[++currentHandle] = event;

#ifdef _DEBUG_EVENTS
    if (!event->debugString.empty())
        LOG(DEBUG) << "registered event " << event->debugString;
#endif

    return currentHandle;
}

void Game::unregisterEvent(Game::Event::Handle handle)
{
#ifdef _DEBUG_EVENTS
    //LOG(DEBUG) << "trying to unregister event handle " << handle;
    if (_events.count(handle))
        LOG(DEBUG) << "unregistered event " << _events[handle]->debugString;
#endif

    _events.erase(handle);
}

// Local functions
namespace {

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO))
        throw SDLException();

    SDL_version version;
    SDL_GetVersion(&version);
    LOG(INFO) << "using SDL " << static_cast<int>(version.major) << "."
              << static_cast<int>(version.minor) << "."
              << static_cast<int>(version.patch);

    auto flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(flags) & flags))
    {
        SDL_Quit();
        throw SDLImageException();
    }
}

void initGL()
{
    auto errorCode = glewInit();
    if (errorCode != GLEW_OK)
    {
        Window::destroy();
        shutdownSDL();
        throw GLEWException(errorCode);
    }

    LOG(INFO) << "using GLEW " << glewGetString(GLEW_VERSION);
    LOG(INFO) << "using OpenGL " << glGetString(GL_VERSION);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    //glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    auto screenMinInWorld = _camera->getScreenMinInWorld();
    auto screenMaxInWorld = _camera->getScreenMaxInWorld();
    gluOrtho2D(screenMinInWorld.x, screenMaxInWorld.x,
               screenMinInWorld.y, screenMaxInWorld.y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
    {
        Window::destroy();
        shutdownSDL();
        throw GLException(errorCode);
    }
}

void shutdownSDL()
{
    IMG_Quit();
    SDL_Quit();
}

void initEntities()
{
    auto walls = std::make_shared<Walls>();
    LOG(DEBUG) << *walls;

    auto initialPosition =
            b2Vec2(_camera->getWorldWidth() / 2,
                   _camera->getWorldHeight() / 8);
    auto paddle = std::make_shared<Paddle>(initialPosition, walls);
    LOG(DEBUG) << *paddle;

    initialPosition =
            b2Vec2(_camera->getWorldWidth() / 2,
                   _camera->getWorldHeight() / 4);
    auto ball = std::make_shared<Ball>(initialPosition);
    LOG(DEBUG) << *ball;

    _entities.push_back(walls);
    _entities.push_back(paddle);
    _entities.push_back(ball);

    _level = std::make_shared<Level>("res/levels/level.dat");



    // Draw the cursor last to preserve draw order
    //auto cursor = std::make_shared<Cursor>();
    //LOG(DEBUG) << *cursor;
    //_entities.push_back(cursor);
}

void destroyEntities()
{
    _entities.clear();
    _level.reset();
    //_deadEntities.clear();
}

void registerEvents()
{
    // Set up an event filter to drop normal keyboard events from the queue
    SDL_SetEventFilter(
            [](void *unused, SDL_Event *e) -> int
            {
                return !(e->type == SDL_KEYDOWN ||
                         e->type == SDL_KEYUP);
            },
            nullptr);

    // Register a custom keyboard event designed to fire every "tick"
    Game::Event::CUSTOM_KEYPRESS_EVENT = SDL_RegisterEvents(1);
    if (Game::Event::CUSTOM_KEYPRESS_EVENT == (Uint32)-1)
        throw SDLException();

    // Window/program quit event
    Game::registerEvent(std::make_shared<Game::QuitEvent>());

    // Escape key handler
    Game::registerEvent(
            SDL_SCANCODE_ESCAPE,
            [](const SDL_Event &e) { Game::setRunning(false); },
            "EscapeEvent");
}

void handleEvents()
{
    // Inject an event to check our keyboard state
    //SDL_PumpEvents();
    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    event.type = Game::Event::CUSTOM_KEYPRESS_EVENT;
    event.user.data1 = const_cast<Uint8 *>(SDL_GetKeyboardState(nullptr));
    SDL_PushEvent(&event);

    // Run the normal event loop
    memset(&event, 0, sizeof(SDL_Event));
    while (SDL_PollEvent(&event))
    {
        for (auto &pair : _events)
        {
            if (pair.second->test(event)) pair.second->fire(event);
        }
    }
}

void updateEntities(float dt)
{
    for (auto entity : _entities)
    {
        entity->update(dt);
    }

    _level->update(dt);
}

void cullDeadEntities()
{
    _entities.remove_if([](std::shared_ptr<Entity> e)
    {
        return e->isMarkedForDeath();
    });

    _level->cullDeadEntities();
}

void drawScene()
{
    Window::clear();

    for (auto entity : _entities)
    {
        entity->draw();
    }

    _level->draw();

    Window::flip();
}

}
