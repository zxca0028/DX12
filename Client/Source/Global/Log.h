#pragma once

#include "Global/GlobalInstance.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace CLIENT
{
#define LOG_TRACE(...)		::CLIENT::Log::TraceLog   (::CLIENT::Log::GetLogger(), __VA_ARGS__);
#define LOG_INFO(...)		::CLIENT::Log::InfoLog    (::CLIENT::Log::GetLogger(), __VA_ARGS__);
#define LOG_WARN(...)		::CLIENT::Log::WarnLog    (::CLIENT::Log::GetLogger(), __VA_ARGS__);
#define LOG_ERROR(...)		::CLIENT::Log::ErrorLog   (::CLIENT::Log::GetLogger(), __VA_ARGS__);
#define LOG_CRITICAL(...)	::CLIENT::Log::CriticalLog(::CLIENT::Log::GetLogger(), __VA_ARGS__);

	class Log final : public ISingleton
	{
		friend class GlobalInstance;
		friend class Application;
	public:
		virtual void Init() override
		{
			LOG_INFO("Complete to Register Log");
		}
	private:
		SharedPtr<spdlog::logger> mLogger = nullptr;
	public:
		inline static SharedPtr<spdlog::logger>& GetLogger() { return GlobalInstance::Instance<Log>()->mLogger; }
	public:
		Log()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");
			mLogger = spdlog::stdout_color_mt("Application ");
			mLogger->set_level(spdlog::level::trace);
		}
		virtual ~Log()
		{
			mLogger.reset();
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