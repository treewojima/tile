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

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "defines.hpp"
#include <iostream>
#include "vector.hpp"

class Camera
{
public:
    Camera(const Vector2f &worldMin, const Vector2f &worldMax,
           const Vector2f &screenMin, const Vector2f &screenMax,
           const Vector2f &screenMinInWorld, const Vector2f &screenMaxInWorld);

    Camera(const Vector2f &worldMax,
           const Vector2f &screenMax,
           const Vector2f &screenMinInWorld,
           const Vector2f &screenMaxInWorld);

    Camera(const Vector2f &worldMax,
           const Vector2f &screenMax);

    void setCamera(const Vector2f &worldMin, const Vector2f &worldMax,
                   const Vector2f &screenMin, const Vector2f &screenMax,
                   const Vector2f &screenMinInWorld, const Vector2f &screenMaxInWorld);

    Vector2f worldToScreen(const Vector2f &vec, bool invertYAxis = false) const;
    Vector2f screenToWorld(const Vector2f &vec, bool invertYAxis = false) const;

    float getWorldWidth() const;
    float getWorldHeight() const;
    float getScreenWidth() const;
    float getScreenHeight() const;
    Vector2f getScreenMinInWorld() const { return _screenMinInWorld; }
    Vector2f getScreenMaxInWorld() const { return _screenMaxInWorld; }

    void setScreenMin(const Vector2f &vec);
    void setScreenMax(const Vector2f &vec);
    void setWorldMin(const Vector2f &vec);
    void setWorldMax(const Vector2f &vec);
    void setScreenMinInWorld(const Vector2f &vec);
    void setScreenMaxInWorld(const Vector2f &vec);

    void reshape(float x1, float y1, float x2, float y2);
    void pan(const Vector2f &vec);
    void zoom(const Vector2f &vec);
    //void rotate(float angle);

    std::string toString() const;

private:
    Vector2f _scaleWorldToScreen, _scaleScreenToWorld;
    Vector2f _screenMin, _screenMax;
    Vector2f _worldMin, _worldMax;
    Vector2f _screenMinInWorld, _screenMaxInWorld;
    //b2Vec2 _rotationOrigin;
    //float _rotation;
};

// Helper stream operator
inline std::ostream &operator<<(std::ostream &stream, const Camera &c)
{
    stream << c.toString();
    return stream;
}

#endif
