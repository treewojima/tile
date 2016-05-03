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

#ifndef __COMPONENTS_BASE_HPP__
#define __COMPONENTS_BASE_HPP__

#include "defines.hpp"

#include <boost/core/demangle.hpp>
#include <memory>
#include <sstream>
#include <string>

#include "entity.hpp"
#include "events/base.hpp"
#include "stringable.hpp"

#ifdef _DEBUG
//#   define _DEBUG_COMPONENTS
#endif

namespace Components
{
    class Base : public Stringable
    {
    protected:
        Base(std::shared_ptr<Entity> parent, const std::string &debugName);

    public:
        virtual ~Base();

        inline std::string getDebugName() const { return _debugName; }
        inline std::shared_ptr<Entity> getParent() const { return _parent; }

        std::string toString() const;

    private:
        std::shared_ptr<Entity> _parent;
        std::string _debugName;
    };
}

namespace Events
{
    template <class T>
    class SpecificComponentCreated : public Events::Base
    {
    public:
        std::shared_ptr<T> component;

        SpecificComponentCreated(std::shared_ptr<T> component_) :
            Events::Base(),
            component(component_) {}

		std::string toString() const
		{
			std::ostringstream ss;
            ss << "Events::SpecificComponentCreated<"
               << boost::core::demangle(typeid(T).name()) << ">"
			   << "[component = " << component << "]";
			return ss.str();
		}
    };

    typedef SpecificComponentCreated<Components::Base> ComponentCreated;
}

#endif
