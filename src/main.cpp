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
#include <easylogging++.h>
#include <SDL2/SDL.h>
#include <tclap/CmdLine.h>
#include "game.hpp"
#include "level.hpp"

_INITIALIZE_EASYLOGGINGPP

// Locals
namespace
{
    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;

    Game::Options parseArgs(int argc, char *argv[]);
}

int main(int argc, char *argv[])
{
    _START_EASYLOGGINGPP(argc, argv);

    try
    {
        Game::run(parseArgs(argc, argv));
    }
    catch (TCLAP::ArgException &e)
    {
        LOG(ERROR) << "ARG EXCEPTION: " << e.error();
        return 1;
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << "EXCEPTION: " << e.what();
        return 1;
    }

    LOG(INFO) << "shut down cleanly";
    return 0;
}

// Local functions
namespace {

Game::Options parseArgs(int argc, char *argv[])
{
    TCLAP::ValueArg<int> argWidth("w",
                                  "width",
                                  "window width",
                                  false,
                                  DEFAULT_WINDOW_WIDTH,
                                  "positive integer value");
    TCLAP::ValueArg<int> argHeight("h",
                                   "height",
                                   "window height",
                                   false,
                                   DEFAULT_WINDOW_HEIGHT,
                                   "positive integer value");
    TCLAP::CmdLine cmdLine("game", ' ', "0.1", false);

    cmdLine.add(argWidth);
    cmdLine.add(argHeight);
    cmdLine.parse(argc, argv);

    auto width = argWidth.getValue();
    auto height = argHeight.getValue();

    std::ostringstream ss;
    if (width < 0)
    {
        ss << "expected positive integer for window width (got "
           << width << ")";
        throw TCLAP::ArgException(ss.str());
    }
    else if (height < 0)
    {
        ss << "expected positive integer for window height (got "
           << height << ")";
        throw TCLAP::ArgException(ss.str());
    }

    return Game::Options { width, height };
}

}
