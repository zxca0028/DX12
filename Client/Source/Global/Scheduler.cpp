#include "pch.h"
#include "Scheduler.h"

namespace CLIENT
{
	void Scheduler::Init()
	{
		LOG_INFO("Complete to Register Scheduler");
	}

	void Scheduler::Update()
	{
		for (auto& taskGroup : mSortedSyncTasks)
		{
			auto& taskList = taskGroup.second;
			u64 count = taskGroup.second.size();

			for (u64 i = 0; i < count; ++i)
			{
				auto task = taskList[i].lock();

				if (task)
				{
					switch (task->GetScheduleType())
					{
					case EScheduleType::SyncByFrame:
					{
						auto frameTask = static_cast<SyncTaskByFrame*>(task.get());
						Update(frameTask);
					}
					break;
					case EScheduleType::SyncByTick:
					{

					}
					break;
					}
				}
			}
		}
	}

	void Scheduler::Update(SyncTaskByFrame* task)
	{
		EScheduleResult result = EScheduleResult::Break;

		result = task->Function();

		switch (result)
		{
		case EScheduleResult::Continue:
			break;
		case EScheduleResult::Break:
			break;
		}
	}

	u64 Scheduler::GetScheduleID()
	{
		return mIDOffset++;
	}

	SharedPtr<ScheduleHandle> Scheduler::ScheduleByFrame(i32 delayFrame, i32 frameCycle, i32 repeat, i32 priority, const SyncTaskFunction& task)
	{
		auto SyncTask = CreateSharedPtr<SyncTaskByFrame>();
		//SyncTask->Delay = delayFrame;
		//SyncTask->FrameCycle = frameCycle;
		//SyncTask->Repeat = repeat;
		SyncTask->Priority = priority;
		SyncTask->Function = task;

		auto handle = CreateSharedPtr<ScheduleHandle>();
		//handle->mState = EScheduleState::Wait;
		//handle->mType = EScheduleType::SyncByFrame;
		//SyncTask->Handle = handle;

		u64 ID = GetScheduleID();

		mSyncTaskPool.emplace(ID, SyncTask);

		mSortedSyncTasks[SyncTask->Priority].push_back(SyncTask);

		return handle;
	}
}