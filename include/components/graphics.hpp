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

#ifndef __COMPONENTS_GRAPHICS_HPP__
#define __COMPONENTS_GRAPHICS_HPP__

#include "defines.hpp"
#include "components/base.hpp"
#include "components/position.hpp"
#include "texture.hpp"
#include "vector.hpp"

namespace Components
{
    class Graphics : public Base
    {
    public:
        Graphics(TextureManager::ConstResourcePtr texture,
                 std::shared_ptr<Components::Position> positionComponent);

    protected:
        Graphics();

    public:
        virtual void draw();

        inline TextureManager::ConstResourcePtr getTexture() const
            { return _texture; }

        std::string toString() const;

    protected:
        void setTexture(TextureManager::ConstResourcePtr texture);

    private:
        TextureManager::ConstResourcePtr _texture;
        std::shared_ptr<Components::Position> _position;
    };
}

#endif
