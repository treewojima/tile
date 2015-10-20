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

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "defines.hpp"

class State
{
public:
	// I don't like this...
	enum class Type
	{
		Main,
		Paused
	};

protected:
	State(Type type) : _type(type) {}

public:
	virtual ~State() {}

	virtual void initialize() {}
	virtual void destroy() {}

	virtual void preLoop() {}
	virtual void postLoop() {}

	virtual void update(float dt) {}
	virtual void draw() {}

	bool isActive() const;
	Type getType() const { return _type; }

private:
	Type _type;
};



#endif
