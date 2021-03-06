#pragma once
#include <memory>
#include <iostream>

#include "spdlog\spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
public:
	static void Init();	

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
};

#ifdef _DEBUG
	#define LOG_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__) 
	#define LOG_ERROR(...) ::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define LOG_WARNING(...) ::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define LOG_INFO(...) ::Log::GetCoreLogger()->info(__VA_ARGS__)

	// optional logger with another name in case needed
	#define APP_CRITICAL(...) ::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define APP_ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__)
	#define APP_WARNING(...) ::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define APP_INFO(...) ::Log::GetClientLogger()->info(__VA_ARGS__)
#else
	#define	LOG_CRITICAL(...)
	#define LOG_ERROR(...)
	#define LOG_WARNING(...)
	#define LOG_INFO(...)

	#define APP_CRITICAL(...)
	#define APP_ERROR(...)
	#define APP_WARNING(...)
	#define APP_INFO(...)
#endif
