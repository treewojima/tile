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

#include "game.hpp"

#include <events/dispatcher.hpp>
#include <states/maingame.hpp>

Game::Game(const Options &options) :
    _options(options),
    _running(false)
{
    Logger::init(options.logFile);
}

void Game::run()
{
    // Initialize SDL
    initSDL();

    // Create the renderer and window
    _renderer = std::make_unique<Graphics::Renderer>();
    _window = _renderer->getWindow();

    // Set up the camera view
    _camera = std::make_unique<Camera>(
                   Vector2f::ZERO,                                        // worldMin
                   Vector2f(_options.windowWidth, _options.windowHeight),   // worldMax
                   Vector2f::ZERO,                                        // screenMin
                   Vector2f(_options.windowWidth, _options.windowHeight),   // screenMax
                   Vector2f::ZERO,                                        // screenMinInWorld
                   Vector2f(_options.windowWidth, _options.windowHeight));  // screenMaxInWorld

    // Initialize entity manager
    _entityMgr = std::make_unique<EntityManager>();
    _entityMgr->initialize();

    // Initialize movement system
    _movementSys = std::make_unique<Systems::Movement>();

    // Set up various managers and miscellaneous objects that haven't been
    // initialized yet
    _fpsTimer = std::make_unique<Timer>();
    _stateMgr = std::make_unique<StateManager>();
    _texMgr = std::make_unique<Graphics::TextureManager>();
    _graphicsSys = std::make_unique<Systems::Graphics>();

    //_map = std::make_unique<Map>("res/desert.tmx");

    // Set up event handling details
    registerEvents();

    // Create the initial main game state
    _stateMgr->push(std::make_shared<States::MainGame>());

    // Main loop variables
    Timer stepTimer;
    float dt = 0;
    unsigned long countedFrames = 0;
    Uint32 oldTicks = 0, currentTicks = 0;

    // Enter the main loop
    setRunning(true);
    _fpsTimer->start();
    stepTimer.start();
    while (isRunning())
    {
        // Update the FPS counter every second
        currentTicks = _fpsTimer->getTicks();
        if (currentTicks - oldTicks >= 1000)
        {
            float avgFPS = countedFrames / (currentTicks / 1000.f);
            std::ostringstream ss;
            ss << "FPS: " << avgFPS;
            _window->setTitle(ss.str());
            oldTicks = currentTicks;
        }

        // Update scene
        try
        {
            // First, make sure we even have a state to begin with
            auto state = _stateMgr->peek();
            if (!state)
            {
                throw Exceptions::EmptyStateStack();
            }

            // Pre-processing
            state->preLoop();

            // Process the event queue
            handleEvents();

            // Update entities, making sure to let them know how long it's been
            // since the last step
            dt = stepTimer.getTicks() / 1000.f;
            state->update(dt);
            _movementSys->update(dt);

            // Update the screen
            _renderer->clear();
            state->draw(dt);
            _renderer->present();

            // Restart timer
            stepTimer.start();

            // Post-processing
            state->postLoop();
        }
        catch (std::exception &e)
        {
            LOG_ERROR << "EXCEPTION: " << e.what();
            setRunning(false);
        }
        catch (...)
        {
            LOG_ERROR << "UNKNOWN/UNHANDLED EXCEPTION";
            setRunning(false);
        }

        // Update frame count for the next iteration
        countedFrames++;
    }
    _fpsTimer->stop();

    // Clean up
    _stateMgr->destroy();
    _movementSys->destroy();
    _graphicsSys->destroy();
    _entityMgr->destroy();
    _texMgr->clear();
    _renderer->destroy();
    shutdownSDL();
}

void Game::exit(const std::exception *e)
{
    std::ostringstream ss;
    ss << "EXCEPTION: ";

    if (e)
    {
        ss << e->what();
    }
    else
    {
        ss << "exit() called";
    }

    LOG_ERROR << ss.str();
    std::exit(1);
}

void Game::onEvent(const Events::Quit &event)
{
    setRunning(false);
    LOG_INFO << "received quit signal";
}

void Game::onEvent(const Events::MouseDown &event)
{
    auto col = event.position.x / 32;
    auto row = (_window->getHeight() - event.position.y) / 32;

    switch (event.button)
    {
    case Events::MouseDown::Button::Left:
        leftClick(col, row);
        break;

    case Events::MouseDown::Button::Right:
        rightClick(col, row);
        break;
    }
}

void Game::initSDL()
{
    // Necessary for SDL_GetKeyboardState()?
    if (SDL_InitSubSystem(SDL_INIT_EVENTS))
    {
        throw Exceptions::Base(SDL_GetError());
    }

    SDL_version version;
    SDL_GetVersion(&version);
    LOG_INFO << "using SDL " << static_cast<int>(version.major) << "."
             << static_cast<int>(version.minor) << "."
             << static_cast<int>(version.patch);

    // The internal SDL keystate pointer never changes, so go ahead and
    // assign it now
    Events::KeyDown::keys = SDL_GetKeyboardState(nullptr);
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

void Game::shutdownSDL()
{
    SDL_Quit();
}

void Game::registerEvents()
{
    // Window/program quit event
    Events::Dispatcher::subscribe<Events::Quit>(*this);

    // Mousedown test
    Events::Dispatcher::subscribe<Events::MouseDown>(*this);
}

void Game::handleEvents()
{
    // Call input event handlers every tick
    Events::Dispatcher::raise<Events::KeyDown>();

    // Run the normal event loop
    SDL_Event event = {0};
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        // Catch special keys and fall through to quit
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                event.type = SDL_QUIT;
                event.quit.timestamp = SDL_GetTicks();
                SDL_PushEvent(&event);
                break;
            }
            break;

        case SDL_QUIT:
            Events::Dispatcher::raise<Events::Quit>();
            break;

        // Capture all mouse down events for now
        case SDL_MOUSEBUTTONDOWN:
            {
                Events::MouseDown::Button button;
                Vector2i position;

                if (event.button.button == SDL_BUTTON_RIGHT)
                    button = Events::MouseDown::Button::Right;
                else if (event.button.button == SDL_BUTTON_LEFT)
                    button = Events::MouseDown::Button::Left;
                else continue;

                position.x = event.button.x;
                position.y = event.button.y;

                Events::Dispatcher::raise<Events::MouseDown>(button, position);
            }
            break;

        default:
            break;
        }
    }
}

void Game::leftClick(int col, int row)
{
    std::ostringstream ss;
    ss << "Entities at location: ";

#if 0
    try
    {
        auto list = _map->getEntitiesAt(col, row);
        int size = list.size();
        if (!size)
        {
            throw std::out_of_range("");
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                ss << list[i]->getDebugName();
                if (i != size - 1) ss << ", ";
            }
        }
    }
    catch (std::out_of_range)
    {
        ss << "<none>";
    }

    LOG_DEBUG << ss.str();
#endif
}

void Game::rightClick(int col, int row)
{
}
