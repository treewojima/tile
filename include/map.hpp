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

#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "defines.hpp"

#include <list>
#include <memory>
#include <string>
#include <tmx/Map.h>

#include "entity.hpp"
#include "exception.hpp"
#include "stringable.hpp"
#include "texture.hpp"

class Map : public Stringable
{
public:
    Map(const std::string &filename);
    ~Map();

	void destroy();

    std::string toString() const;

private:
    class LayerVisitor : public tmx::LayerVisitor
    {
    public:
        LayerVisitor(Map *parent) : _parent(parent) { assert(parent); }

        void visitTileLayer(const tmx::Map &map, const tmx::TileLayer &layer) override;

    private:
        Map *_parent;
    };

	bool _destroyed;
	std::string _filename;
    std::unique_ptr<tmx::Map> _map;
    std::list<std::shared_ptr<Entity>> _entities;

    void loadTilesetTextures();
};

class MapException : public Exception
{
public:
	using Exception::Exception;
};

#endif
