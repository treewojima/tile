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
#include <GL/glew.h>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifdef _ENABLE_AUDIO
#   include <SDL2/SDL_mixer.h>
#endif
#include <sstream>

#include "colors.hpp"
#include "components/animatedsprite.hpp"
#include "components/position.hpp"
#include "components/staticsprite.hpp"
#include "entities/tux.hpp"
#include "exception.hpp"
#include "graphics.hpp"
#include "helper_events.hpp"
#include "level.hpp"
#include "logger.hpp"
#include "map.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "window.hpp"

Uint32 Game::Event::CUSTOM_KEYPRESS_EVENT;

// Locals
namespace
{
    // Main loop status flag
    bool _running = false;

    // Map of event handles to event instances
    std::map<Game::Event::Handle, std::shared_ptr<Game::Event>> _events;

    // List of game entities
    std::list<std::shared_ptr<Entity>> _entities;
    std::unique_ptr<Map> _map;

    // Timer for tracking and calculating FPS
    Timer _fpsTimer;

    // Camera view
    std::unique_ptr<Camera> _camera;

    // Resource managers
    TextureManager _texMgr;

    // Subsystem initialization and shutdown functions
    void initSDL();
    void initGL();
#ifdef _ENABLE_AUDIO
    void initAudio();
#endif
    void shutdownSDL();

    // Resource and entity allocation/destruction/management functions
    void loadTextures();
    void createEntities();
    void updateEntities(float dt);
    void cullDeadEntities();
    void destroyEntities();

    // Event handling functions
    void registerEvents();
    void handleEvents();

    // Scene update functions
    void drawScene();
}

void Game::run(const Game::Options &options)
{
    // Initialize SDL and create the window
    initSDL();
    Window::create(options.windowWidth, options.windowHeight, options.vsync);

    // Set up the camera view
    //auto screenRatio = static_cast<float>(options.windowWidth) /
    //                   static_cast<float>(options.windowHeight);
    _camera = std::unique_ptr<Camera>(
        new Camera(Vector2f::ZERO,                                        // worldMin
                   Vector2f(options.windowWidth, options.windowHeight),   // worldMax
                   Vector2f::ZERO,                                        // screenMin
                   Vector2f(options.windowWidth, options.windowHeight),   // screenMax
                   Vector2f::ZERO,                                        // screenMinInWorld
                   Vector2f(options.windowWidth, options.windowHeight))); // screenMaxInWorld
    //BOOST_LOG_TRIVIAL(debug) << *_camera;

    // Further initialization for OpenGL and audio
    initGL();
#ifdef _ENABLE_AUDIO
    initAudio();
#endif

    // Set up event handling details
    registerEvents();

    // Load resources and create entities
    loadTextures();
    createEntities();

    // Main loop variables
    _map = std::unique_ptr<Map>(new Map("desert", "res/desert.tmx"));
    Timer stepTimer;
    float dt = 0;
    unsigned long countedFrames = 0;
    Uint32 oldTicks = 0, currentTicks = 0;

    // Enter the main loop
    setRunning(true);
    _fpsTimer.start();
    stepTimer.start();
    while (isRunning())
    {
        // Update the FPS counter every second
        currentTicks = _fpsTimer.getTicks();
        if (currentTicks - oldTicks >= 1000)
        {
            float avgFPS = countedFrames / (currentTicks / 1000.f);
            std::ostringstream ss;
            ss << "FPS: " << avgFPS;
            Window::setTitle(ss.str());
            oldTicks = currentTicks;
        }

        // Update scene
        try
        {
            // Process the event queue
            handleEvents();

            // Update entities, making sure to let them know how long it's been
            // since the last step
            dt = stepTimer.getTicks() / 1000.f;
            updateEntities(dt);
            stepTimer.start();

            // Update the screen
            drawScene();

            // Collect dead entities
            cullDeadEntities();
        }
        catch (std::exception &e)
        {
            LOG_ERROR << "EXCEPTION: " << e.what();
            setRunning(false);
        }

        // Update frame count for the next iteration
        countedFrames++;
    }
    _fpsTimer.stop();

    // Clean up

    // NOTE: Why does the game hang when this line is omitted?
    if (_map.get() != nullptr) _map.reset();

    destroyEntities();
    _texMgr.clear();
    Window::destroy();
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

TextureManager &Game::getTexMgr()
{
    return _texMgr;
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
        LOG_DEBUG << "registered event " << event->debugString;
#endif

    return currentHandle;
}

void Game::unregisterEvent(Game::Event::Handle handle)
{
#ifdef _DEBUG_EVENTS
    //BOOST_LOG_TRIVIAL(debug) << "trying to unregister event handle " << handle;
    if (_events.count(handle))
        LOG_DEBUG << "unregistered event " << _events[handle]->debugString;
#endif

    _events.erase(handle);
}

Game::Event::~Event()
{
#ifdef _DEBUG_EVENTS
		LOG_DEBUG << "destroyed event \"" << debugString << "\"";
#endif
}

// Local functions
namespace {

void initSDL()
{
	int flags = SDL_INIT_VIDEO;
#ifdef _ENABLE_AUDIO
	flags |= SDL_INIT_AUDIO;
#endif
	if (SDL_Init(flags))
	{
		throw SDLException();
	}

    SDL_version version;
    SDL_GetVersion(&version);
    LOG_INFO << "using SDL " << static_cast<int>(version.major) << "."
             << static_cast<int>(version.minor) << "."
             << static_cast<int>(version.patch);

    flags = IMG_INIT_PNG | IMG_INIT_JPG;
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

    LOG_INFO << "using GLEW " << glewGetString(GLEW_VERSION);
    LOG_INFO << "using OpenGL " << glGetString(GL_VERSION);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    // Enable alpha blending/transparency
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

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

#ifdef _ENABLE_AUDIO
void initAudio()
{
    static const int DEFAULT_AUDIO_FREQ = 44100;

    auto ret = Mix_OpenAudio(DEFAULT_AUDIO_FREQ, // audio frequency
                             MIX_DEFAULT_FORMAT, // sample format
                             2,                  // number of channels
                             2048);              // sample size in bytes
    if (ret)
    {
        std::ostringstream ss;
        ss << "Could not initialize SDL_Mixer: " << Mix_GetError();
        throw SDLMixerException(ss.str());
    }
}
#endif

void shutdownSDL()
{
#ifdef _ENABLE_AUDIO
    Mix_Quit();
#endif
    IMG_Quit();
    SDL_Quit();
}

void loadTextures()
{
    const SDL_Color colorKey = Colors::makeColor(255, 0, 255);
    TextureManager::ResourcePtr texture;

    //texture = std::make_shared<Texture>("tux", "res/tux.png", &colorKey);
    //_texMgr.add(texture->getName(), texture);

    //texture = std::make_shared<Texture>("beastie", "res/beastie.png", &colorKey);
    //_texMgr.add(texture->getName(), texture);

    for (int i = 1; i <= 4; i++)
    {
        std::ostringstream ss;
        ss << "foo" << i;
        texture = std::make_shared<Texture>(ss.str(), "res/" + ss.str() + ".png", &colorKey);
        _texMgr.add(texture->getName(), texture);
    }
}

void createEntities()
{
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
        textures.push_back(_texMgr.get(ss.str()));
    }

    tux->graphics = std::make_shared<Components::AnimatedSprite>(textures,
                                                                 tux->position);

    _entities.push_back(tux);
}

void updateEntities(float dt)
{
    for (auto entity : _entities)
    {
        entity->update(dt);
    }
}

void cullDeadEntities()
{
    _entities.remove_if([](std::shared_ptr<Entity> e)
    {
        return e->isMarkedForDeath();
    });
}

void destroyEntities()
{
    _entities.clear();
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
    SDL_Event event = {0};
    event.type = Game::Event::CUSTOM_KEYPRESS_EVENT;
    event.user.data1 = const_cast<Uint8 *>(SDL_GetKeyboardState(nullptr));
    SDL_PushEvent(&event);

    // Run the normal event loop
    event = {0};
    while (SDL_PollEvent(&event))
    {
        for (auto &pair : _events)
        {
            if (pair.second->test(event)) pair.second->fire(event);
        }
    }
}

void drawScene()
{
    Window::clear(255, 255, 255);

    if (_map.get() != nullptr) _map->draw();

    for (auto entity : _entities)
    {
        if (entity->graphics.get() != nullptr)
        {
            entity->graphics->draw();
        }
    }

    //Graphics::blitTexture(Game::getTexMgr().get("desert-138"), 50, 50);

    Window::flip();
}

}
