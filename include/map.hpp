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

#include <memory>
#include <string>
#include <tmx/Map.h>
#include <vector>

#include "components/mapposition.hpp"
#include "events/subscriber.hpp"
#include "graphics/texture.hpp"
#include "exceptions.hpp"
#include "stringable.hpp"

#ifdef _DEBUG
#   define DEBUG_MAP
#endif

class Map : public Events::Subscriber
{
public:
    Map(const std::string &filename);
    ~Map();

	void destroy();

    void preprocess();

    typedef std::vector<std::shared_ptr<Entity>> EntityList;
    EntityList getEntitiesAt(int col, int row);

    void onEvent(const Events::MapPositionComponentCreated &event);

    std::string toString() const;

	static const int TILE_WIDTH, TILE_HEIGHT;

private:
    class LayerVisitor : public tmx::LayerVisitor
    {
    public:
        LayerVisitor(Map *parent) : _parent(parent) { assert(parent); }

        void visitTileLayer(const tmx::Map &map, const tmx::TileLayer &layer) override;

    private:
        Map *_parent;
    };

	std::string _filename;
    std::unique_ptr<tmx::Map> _map;

    std::shared_ptr<Graphics::Texture> _preprocessedTexture;

    //std::list<std::shared_ptr<Entity>> _entities;
    //std::list<std::shared_ptr<Components::MapPosition>> _components;

    typedef std::vector<EntityList> GridRow;
    typedef std::vector<GridRow> EntityGrid;
    EntityGrid _entityGrid;

    void loadTilesetTextures();
};

namespace Exceptions
{
    class MapException : public Base
    {
    public:
        using Base::Base;
    };
}

#endif
