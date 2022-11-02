#pragma once

#include "Core.h"

namespace CLIENT
{
	class SchedulePriority
	{
	public:
		enum
		{
			Graphics_BeginFrame = 1,
			Graphics_EndFrame   = 2
		};
	};
	enum class EScheduleState
	{
		Wait,
		Run,
		Complete
	};
	enum class EScheduleType
	{
		SyncByTick,
		SyncByFrame,
		Async
	};
	enum class EScheduleResult
	{
		Continue,
		Break
	};

	class ScheduleHandle
	{
	};

	using SyncTaskFunction = std::function<EScheduleResult()>;
	class Scheduler final : public ISingleton
	{
		friend class ISingleton;
		friend class Application;
	private:
		class SyncTask
		{
		public:
			i32 Priority = 0;
			SyncTaskFunction Function;
		public:
			virtual ~SyncTask() = default;
		public:
			virtual EScheduleType GetScheduleType() const = 0;
		};
		class SyncTaskByFrame : public SyncTask
		{
		public:
			virtual ~SyncTaskByFrame() = default;
			virtual EScheduleType GetScheduleType() const
			{
				return EScheduleType::SyncByFrame;
			}
		};
	public:
		~Scheduler()
		{
			mSortedSyncTasks.clear();
			mSyncTaskPool.clear();
		}
	private:
		u64 mIDOffset = 0;
		Dictionary<u64, SharedPtr<SyncTask>> mSyncTaskPool;
		SortedDictionary<i32, vector<WeakPtr<SyncTask>>> mSortedSyncTasks;
	private:
		void Init();
		void Update();
		void Update(SyncTaskByFrame* task);
		u64 GetScheduleID();
	public:
		SharedPtr<ScheduleHandle> ScheduleByFrame(i32 delayFrame, i32 frameCycle, i32 repeat, i32 priority, const SyncTaskFunction& task);
	};
}