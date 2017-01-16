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

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include "defines.hpp"

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>

namespace Logger
{
    typedef const char *SeverityType;
    namespace Severity
    {
        extern SeverityType Plain;
        extern SeverityType Debug;
        extern SeverityType Info;
        extern SeverityType Warning;
        extern SeverityType Error;
    }

    void init(const std::string &logFile);
    void destroy();
}

BOOST_LOG_GLOBAL_LOGGER(gLog, boost::log::sources::severity_logger_mt<Logger::SeverityType>)

#define LOG_PLAIN   BOOST_LOG_SEV(gLog::get(), Logger::Severity::Plain)
#define LOG_DEBUG   BOOST_LOG_SEV(gLog::get(), Logger::Severity::Debug)
#define LOG_INFO    BOOST_LOG_SEV(gLog::get(), Logger::Severity::Info)
#define LOG_WARNING BOOST_LOG_SEV(gLog::get(), Logger::Severity::Warning)
#define LOG_ERROR   BOOST_LOG_SEV(gLog::get(), Logger::Severity::Error)

#endif
