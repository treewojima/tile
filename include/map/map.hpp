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

#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "defines.hpp"

#include <tmx/Map.h>

#include "graphics/surface.hpp"
#include "map/tileset.hpp"
#include "resourcemanager.hpp"

namespace Map
{
    class Map
    {
    public:
        static const int TILE_WIDTH, TILE_HEIGHT;

        Map(const std::string &filename);
        ~Map();

    private:
        class LayerVisitor : public tmx::LayerVisitor
        {
        public:
            LayerVisitor(const Map *parent) : _parent(parent)
            {
                assert(parent);
            }

            void visitTileLayer(const tmx::Map &map,
                                const tmx::TileLayer &layer) override;

        private:
            const Map *_parent;
        };

        std::unique_ptr<tmx::Map> _map;
        std::map<std::string, std::shared_ptr<Tileset>> _tilesets;

        void loadTilesets();
    };
}

namespace Exceptions
{
    class MapException : public Base
    {
    public:
        using Base::Base;
    };
}

#endif
