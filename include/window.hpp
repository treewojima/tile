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

#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "defines.hpp"
#include <memory>

namespace Window
{
    void create(int width, int height);
    void destroy();

    void clear(float r = 0, float g = 0, float b = 0, float a = 0);
    //void blitTexture(const Texture &texture, int x, int y);
    void flip();

    void setTitle(const std::string &title);

    //float getWidth();
    //float getHeight();
    //int getWidthPixels();
    //int getHeightPixels();
}

#endif
