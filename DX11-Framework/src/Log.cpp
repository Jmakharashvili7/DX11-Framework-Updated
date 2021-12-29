#include "Log.h"

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init()
{
	// Set the logger pattern as follows
	// %^ sets the color 
	// &T is the time
	// %n is the name of the logger
	// %v is the message to print
	spdlog::set_pattern("%^[%T]%n: %v%$");

	// Set the name and the level of warning for the logger
	s_CoreLogger = spdlog::stdout_color_mt("CORE");
	s_CoreLogger->set_level(spdlog::level::trace);

	// Set the name and the level of warning for the logger
	s_ClientLogger = spdlog::stdout_color_mt("APP");
	s_ClientLogger->set_level(spdlog::level::trace);
}
