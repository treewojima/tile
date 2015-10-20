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

#ifndef __STATEMANAGER_HPP__
#define __STATEMANAGER_HPP__

#include "defines.hpp"

#include <memory>
#include <stack>

#include "exception.hpp"
#include "state.hpp"

class StateManager
{
public:
	StateManager() : _destroyed(false) {}
	~StateManager();
	void destroy();

	typedef std::shared_ptr<State> StatePtr;

	void push(StatePtr state);
	StatePtr pop();
	StatePtr peek() const;

private:
	typedef std::stack<StatePtr> StateStack;

	bool _destroyed;
	StateStack _stateStack;
};

class EmptyStateStackException : public Exception
{
public:
	EmptyStateStackException() : Exception("State stack is empty") {}
};

#endif