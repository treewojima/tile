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

#ifndef __COMPONENTS_ACTIONQUEUE_HPP__
#define __COMPONENTS_ACTIONQUEUE_HPP__

#include "defines.hpp"
#include "components/base.hpp"

// TODO: look into refactoring some of this code as a generic "event queue"

namespace Components
{
	class ActionQueue : public Base
	{
	public:
		using Base::Base;

		std::string toString() const;
	};

	typedef std::shared_ptr<ActionQueue> ActionQueuePtr;
}

#endif
