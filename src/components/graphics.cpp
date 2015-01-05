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
#include "components/graphics.hpp"
#include "graphics.hpp"

#if 0
class Graphics : public Base
{
public:
    Graphics(TextureManager::ConstResourcePtr texture);

    virtual void draw();

    inline TextureManager::ConstResourcePtr getTexture() const
        { return _texture; }

    std::string toString() const;

protected:
    void setTexture(TextureManager::ConstResourcePtr texture);

private:
    TextureManager::ConstResourcePtr _texture;
};
#endif

Components::Graphics::Graphics(TextureManager::ConstResourcePtr texture,
                               std::shared_ptr<Components::Position> positionComponent) :
    Components::Base("graphics"),
    _position(positionComponent)
{
    setTexture(texture);

#ifdef _DEBUG_COMPONENTS
    LOG(DEBUG) << "created component " << toString();
#endif
}

Components::Graphics::Graphics() :
    Components::Base("graphics"),
    _texture(nullptr),
    _position(nullptr)
{
#ifdef _DEBUG_COMPONENTS
    LOG(DEBUG) << "created component " << toString();
#endif
}

void Components::Graphics::draw()
{
    if (getTexture().get() != nullptr && _position.get() != nullptr)
    {
        ::Graphics::blitTexture(getTexture(), _position->x, _position->y);
    }
}

std::string Components::Graphics::toString() const
{
    std::ostringstream ss;
    ss << "Components::Graphics[name = \"" << getName()
       << "\", texture = "
       << (_texture.get() != nullptr ? _texture->toString() : "<null>")
       << ", positionComponent = "
       << (_position.get() != nullptr ? _position->toString() : "<null>")
       << "]";
    return ss.str();
}

void Components::Graphics::setTexture(TextureManager::ConstResourcePtr texture)
{
    _texture = texture;
}
