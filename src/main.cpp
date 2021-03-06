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

#include "defines.hpp"

#include <boost/filesystem/path.hpp>
#ifndef _NO_PROGRAM_OPTIONS
#   include <boost/program_options.hpp>
#endif

#include "game.hpp"
#include "logger.hpp"

// Locals
namespace
{
    const unsigned DEFAULT_WINDOW_WIDTH  = 640;
    const unsigned DEFAULT_WINDOW_HEIGHT = 480;

    Game::Options parseArgs(int argc, char *argv[]);

    std::unique_ptr<Game> _game;
}

Game &getGame()
{
    return *_game;
}

int main(int argc, char *argv[])
{
    try
    {
        auto options = parseArgs(argc, argv);

        _game = std::make_unique<Game>(options);
        _game->run();
    }
    catch (std::exception &e)
    {
        LOG_ERROR << "EXCEPTION: " << e.what();
    }

    LOG_INFO << "shut down cleanly";
    return 0;
}

// Local functions
namespace {

Game::Options parseArgs(int argc, char *argv[])
{
#ifndef _NO_PROGRAM_OPTIONS
    namespace fs = boost::filesystem;
    namespace po = boost::program_options;

    Game::Options options;

    fs::path programPath(argv[0]);
    options.programName = programPath.filename().replace_extension().string();

    std::ostringstream defaultLogFile;
    defaultLogFile << "logs/" << options.programName << ".txt";

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("width,w", po::value<unsigned>(&options.windowWidth)->default_value(DEFAULT_WINDOW_WIDTH), "set window width")
        ("height,h", po::value<unsigned>(&options.windowHeight)->default_value(DEFAULT_WINDOW_HEIGHT), "set window height")
        ("vsync,v", "enable vsync")
        ("log-file", po::value<std::string>(&options.logFile)->default_value(defaultLogFile.str()), "set output log file");

    po::variables_map map;
    po::store(po::parse_command_line(argc, argv, desc), map);
    po::notify(map);

    if (map.count("help"))
    {
        std::cout << desc << std::endl;
        std::exit(0);
    }

    options.vsync = map.count("vsync");
    /*if (map.count("vsync"))
    {
        options.vsync = true;
    }
    else
    {
        options.vsync = false;
    }*/

    return options;
#else
#warning "building w/o program options"
    Game::Options options;
    options.logFile = "logs/tile.txt";
    options.programName = "tile";
    options.vsync = true;
    options.windowHeight = DEFAULT_WINDOW_HEIGHT;
    options.windowWidth = DEFAULT_WINDOW_WIDTH;
    return options;
#endif
}

}
