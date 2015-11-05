#include "defines.hpp"

#ifndef _USE_NEW_COMPONENTS

#include "components/animatedsprite.hpp"

#include "colors.hpp"
#include "exception.hpp"
#include "graphics.hpp"

Components::AnimatedSprite::AnimatedSprite(
        TextureList textures,
        std::shared_ptr<Components::Position> positionComponent) :
    Graphics(),
    _textures(textures),
    _currentTextureIter(_textures.begin()),
    _position(positionComponent)
{
    if (_textures.empty())
    {
        throw Exception("Empty texture list passed to AnimatedSprite component");
    }

    _timer.start();
}

void Components::AnimatedSprite::draw()
{
    if (_timer.getTicks() > 100)
    {
        if (++_currentTextureIter == _textures.end())
            _currentTextureIter = _textures.begin();

        _timer.start();
    }

    ::Graphics::blitTexture(*_currentTextureIter, _position->x, _position->y);
}

std::string Components::AnimatedSprite::toString() const
{
    std::ostringstream ss;
    ss << "Components::AnimatedSprite[name = \"" << getName()
       << "\", textures = <TODO: IMPLEMENT>"
       << ", positionComponent = "
       << (_position.get() != nullptr ? _position->toString() : "<null>")
       << "]";
    return ss.str();
}

#endif
