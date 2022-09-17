#pragma once

#include "Global/SingletonManager.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace CLIENT
{
	class Logger final : public ISingleton<Logger>
	{
	private:
		SharedPtr<spdlog::logger> mLogger = nullptr;
	public:
		Logger()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");
			mLogger = spdlog::stdout_color_mt("App");
		}
		~Logger()
		{
			spdlog::shutdown();
		}
	public:
		template<class ...T>
		inline static void TraceLog(SharedPtr<spdlog::logger> logger, T ... args)
		{
			logger->trace(Convert(args)...);
		}
		template<class ...T>
		inline static void InfoLog(SharedPtr<spdlog::logger> logger, T ... args)
		{
			logger->info(Convert(args)...);
		}
		template<class ...T>
		inline static void WarnLog(SharedPtr<spdlog::logger> logger, T ... args)
		{
			logger->warn(Convert(args)...);
		}
		template<class ...T>
		inline static void ErrorLog(SharedPtr<spdlog::logger> logger, T ... args)
		{
			logger->error(Convert(args)...);
		}
		template<class ...T>
		inline static void CriticalLog(SharedPtr<spdlog::logger> logger, T ... args)
		{
			logger->critical(Convert(args)...);
		}
	private:
		template<class T>
		static auto Convert(T&& arg)
		{
			return std::forward<T>(arg);
		}
	};
}