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
#include "camera.hpp"

#include <sstream>

Camera::Camera(const Vector2f &worldMin, const Vector2f &worldMax,
               const Vector2f &screenMin, const Vector2f &screenMax,
               const Vector2f &screenMinInWorld, const Vector2f &screenMaxInWorld)
{
    setCamera(worldMin, worldMax,
              screenMin, screenMax,
              screenMinInWorld, screenMaxInWorld);
    //_rotationOrigin = b2Vec2();
    //_rotation = 0;
}

Camera::Camera(const Vector2f &worldMax,
               const Vector2f &screenMax,
               const Vector2f &screenMinInWorld,
               const Vector2f &screenMaxInWorld)
{
    setCamera(Vector2f::ZERO, worldMax,
              Vector2f::ZERO, screenMax,
              screenMinInWorld, screenMaxInWorld);
    //_rotationOrigin = b2Vec2();
    //_rotation = 0;
}

Camera::Camera(const Vector2f &worldMax,
               const Vector2f &screenMax)
{
    setCamera(Vector2f::ZERO, worldMax,
              Vector2f::ZERO, screenMax,
              Vector2f::ZERO, worldMax);
}

void Camera::setCamera(const Vector2f &worldMin, const Vector2f &worldMax,
                       const Vector2f &screenMin, const Vector2f &screenMax,
                       const Vector2f &screenMinInWorld, const Vector2f &screenMaxInWorld)
{
    _scaleWorldToScreen.x = (screenMax.x - screenMin.x) /
                            (screenMaxInWorld.x - screenMinInWorld.x);
    _scaleWorldToScreen.y = (screenMax.y - screenMin.y) /
                            (screenMaxInWorld.y - screenMinInWorld.y);

    _scaleScreenToWorld.x = 1.f / _scaleWorldToScreen.x;
    _scaleScreenToWorld.y = 1.f / _scaleWorldToScreen.x;

    _screenMin = screenMin;
    _screenMax = screenMax;
    _worldMin = worldMin;
    _worldMax = worldMax;
    _screenMinInWorld = screenMinInWorld;
    _screenMaxInWorld = screenMaxInWorld;
}

Vector2f Camera::worldToScreen(const Vector2f &vec, bool invertYAxis) const
{
    Vector2f screen;
    screen.x = _screenMin.x + _scaleWorldToScreen.x *
            (vec.x - _worldMin.x - _screenMinInWorld.x);
    screen.y = _screenMin.y + _scaleWorldToScreen.y *
            (vec.y - _worldMin.y - _screenMinInWorld.y);

    if (invertYAxis)
    {
        screen.y = getScreenHeight() - screen.y;
    }

    return screen;
}

Vector2f Camera::screenToWorld(const Vector2f &vec, bool invertYAxis) const
{
    Vector2f world;
    world.x = (vec.x - _screenMin.x) * _scaleScreenToWorld.x +
            _worldMin.x + _screenMinInWorld.x;

    if (invertYAxis)
    {
        // Is this right?
        world.y = ((getScreenHeight() - vec.y) - _screenMin.y) * _scaleScreenToWorld.y +
                _worldMin.y + _screenMinInWorld.y;
    }
    else
    {
        world.y = (vec.y - _screenMin.y) * _scaleScreenToWorld.y +
                _worldMin.y + _screenMinInWorld.y;
    }

    return world;
}

float Camera::getWorldWidth() const
{
    return _worldMax.x - _worldMin.y;
}

float Camera::getWorldHeight() const
{
    return _worldMax.y - _worldMin.y;
}

float Camera::getScreenWidth() const
{
    return _screenMax.x - _screenMin.y;
}

float Camera::getScreenHeight() const
{
    return _screenMax.y - _screenMin.y;
}

void Camera::setScreenMin(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              vec, _screenMax,
              _screenMinInWorld, _screenMaxInWorld);
}

void Camera::setScreenMax(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              _screenMin, vec,
              _screenMinInWorld, _screenMaxInWorld);
}

void Camera::setWorldMin(const Vector2f &vec)
{
    setCamera(vec, _worldMax,
              _screenMin, _screenMax,
              _screenMinInWorld, _screenMaxInWorld);
}

void Camera::setWorldMax(const Vector2f &vec)
{
    setCamera(_worldMin, vec,
              _screenMin, _screenMax,
              _screenMinInWorld, _screenMaxInWorld);
}

void Camera::setScreenMinInWorld(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              _screenMin, _screenMax,
              vec, _screenMaxInWorld);
}

void Camera::setScreenMaxInWorld(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              _screenMin, _screenMax,
              _screenMinInWorld, vec);
}

void Camera::reshape(float x1, float y1, float x2, float y2)
{
    setCamera(_worldMin, _worldMax,
              Vector2f(x1, y1), Vector2f(x2, y2),
              _screenMinInWorld, _screenMaxInWorld);
}

void Camera::pan(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              _screenMin, _screenMax,
              _screenMinInWorld + vec, _screenMaxInWorld + vec);
}

void Camera::zoom(const Vector2f &vec)
{
    setCamera(_worldMin, _worldMax,
              _screenMin, _screenMax,
              _screenMinInWorld + vec, _screenMaxInWorld - vec);
}

std::string Camera::toString() const
{
    std::ostringstream ss;
    ss << "Camera[worldMin = " << _worldMin << ", worldMax = " << _worldMax
       << ", screenMin = " << _screenMin << ", screenMax = " << _screenMax
       << ", screenMinInWorld = " << _screenMinInWorld
       << ", screenMaxInWorld = " << _screenMaxInWorld
       << ", scaleWorldToScreen = " << _scaleWorldToScreen
       << ", scaleScreenToWorld = " << _scaleScreenToWorld << "]";
    return ss.str();
}
