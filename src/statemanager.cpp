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

#include "statemanager.hpp"

StateManager::~StateManager()
{
    destroy();
}

void StateManager::destroy()
{
    static bool destroyed = false;
    if (destroyed) return;

	while (!_stateStack.empty())
	{
		pop();
	}

    destroyed = true;
}

void StateManager::push(StatePtr state)
{
	state->initialize();

	// NOTE: should this check for duplicate states?
	_stateStack.push(state);
}

StateManager::StatePtr StateManager::pop()
{
	auto state = peek();
    if (!state)
        throw Exceptions::EmptyStateStack();

	_stateStack.pop();
	state->destroy();
	
	return state;
}

StateManager::StatePtr StateManager::peek() const
{
	return (_stateStack.empty() ? nullptr : _stateStack.top());
}
