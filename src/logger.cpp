#include "defines.hpp"
#include "logger.hpp"

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup.hpp>
//#include <boost/utility/empty_deleter.hpp>
#include <fstream>
#include <iostream>

#include "game.hpp"

namespace bl = boost::log;
namespace attr = bl::attributes;
namespace expr = bl::expressions;
namespace src = bl::sources;

namespace Logger
{
	namespace Severity
	{
		SeverityType Debug = "Debug";
		SeverityType Info = "Info";
		SeverityType Warning = "Warning";
		SeverityType Error = "Error";
	}
}

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Logger::SeverityType)

BOOST_LOG_GLOBAL_LOGGER_INIT(gLog, src::severity_logger_mt<Logger::SeverityType>)
{
	src::severity_logger_mt<Logger::SeverityType> lg;

	//lg.add_attribute("TimeStamp", attr::local_clock());

	return lg;
}

void Logger::init(const std::string &logFile)
{
	bl::core::get()->add_global_attribute("TimeStamp", attr::local_clock());
	
	typedef bl::sinks::synchronous_sink<bl::sinks::text_ostream_backend> text_sink;
    auto sink = boost::make_shared<text_sink>();

	// File stream
    sink->locked_backend()->add_stream(
                boost::make_shared<std::ofstream>(logFile));

	// std::cout stream
    sink->locked_backend()->add_stream(
                boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

	// Flush after each log entry
	sink->locked_backend()->auto_flush();

	// Set the log format
	auto formatter = expr::stream
		<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%m-%d-%Y %H:%M:%S") << "] "
		<< "<" << expr::attr<Logger::SeverityType>("Severity") << "> "
		<< expr::smessage;
	sink->set_formatter(formatter);

#ifndef _DEBUG
    sink->set_filter(severity != Logger::Severity::Debug);
#endif

	bl::core::get()->add_sink(sink);
}

void Logger::destroy()
{
    bl::core::get()->remove_all_sinks();
}
