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

#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#define _DEBUG
#define _USE_MATH_DEFINES

#define _USE_NEW_ENTITY
#define _USE_NEW_COMPONENTS
//#define _NO_PROGRAM_OPTIONS

//#define _ENABLE_AUDIO

#ifdef _DEBUG
//#   define _TEST_TEXTURE_DIMENSIONS
//#   define _DEBUG_EVENTS
//#   define _DEBUG_TEXTURES
#   define _DEBUG_ENTITIES
#   define _DEBUG_COMPONENTS
#	define _DEBUG_NEW_EVENTS
#endif

#endif
