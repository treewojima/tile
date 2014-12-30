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

#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <easylogging++.h>
#include <sstream>
#include <string>
#include <typeinfo>

template <class T>
class Vector2
{
public:
    T x, y;
    static const Vector2<T> ZERO;

    Vector2() : x(0), y(0) {}
    Vector2(const T &x_, const T &y_) : x(x_), y(y_) {}
    Vector2(const Vector2<T> &v) : x(v.x), y(v.y) {}

    std::string toString() const
    {
        std::ostringstream ss;
        ss << "Vector2<" << typeid(T).name() << ">(" << x << ", " << y << ")";
        return ss.str();
    }

    // TODO: Add more operators as necessary

    inline Vector2<T> &operator+=(const T &scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    inline Vector2<T> &operator+=(const Vector2<T> &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    inline Vector2<T> &operator-=(const T &scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    inline Vector2<T> &operator-=(const Vector2<T> &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

template <class T>
inline Vector2<T> operator+(Vector2<T> lhs, const T &rhs)
{
    lhs += rhs;
    return lhs;
}

template <class T>
inline Vector2<T> operator+(Vector2<T> lhs, const Vector2<T> &rhs)
{
    lhs += rhs;
    return lhs;
}

template <class T>
inline Vector2<T> operator-(Vector2<T> lhs, const T &rhs)
{
    lhs -= rhs;
    return lhs;
}

template <class T>
inline Vector2<T> operator-(Vector2<T> lhs, const Vector2<T> &rhs)
{
    lhs -= rhs;
    return lhs;
}

// Helper stream operators
template <class T, class U>
inline std::ostream &operator<<(std::ostream &stream, const Vector2<U> &v)
{
    stream << v.toString();
    return stream;
}

template <class T>
inline MAKE_LOGGABLE(Vector2<T>, v, stream)
{
    stream << v.toString();
    return stream;
}

#endif
