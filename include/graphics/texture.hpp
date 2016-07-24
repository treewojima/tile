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

#ifndef __GRAPHICS_TEXTURE_HPP__
#define __GRAPHICS_TEXTURE_HPP__

#include "defines.hpp"

#include "color.hpp"
#include "graphics/surface.hpp"
#include "resourcemanager.hpp"
#include "stringable.hpp"
#include "vector.hpp"

#ifdef _DEBUG
//#   define _TEST_TEXTURE_DIMENSIONS
//#   define _DEBUG_TEXTURES
#endif

namespace Graphics
{
    class Texture : public Stringable
    {
        friend class Renderer;

    public:
        // From Surface
        static Texture *create(const std::string &name,
                               const Surface *surface);
        // From file
        static Texture *create(const std::string &name,
                               const std::string &filename,
                               const SDL_Color &colorKey = Color::COLOR_KEY);
        // Blank/filled color
        static Texture *create(const std::string &name,
                               const Vector2i &dimensions,
                               const SDL_Color &color = Color::WHITE);

    private:
        Texture(const std::string &name,
                Surface *surface);

    public:
        ~Texture();

        inline std::string getName() const { return _name; }
        Vector2i getDimensions() const;
        inline unsigned getWidth() const { return getDimensions().x; }
        inline unsigned getHeight() const { return getDimensions().y; }

        std::string toString() const;

    private:
        std::string _name;
        SDL_Texture *_texture;
    };

    // Texture resource manager type
    typedef ResourceManager<std::shared_ptr<Texture>> TextureManager;
}

#endif
