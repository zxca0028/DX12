#pragma once

#include "Common/Define.h"

namespace CLIENT
{
	class GlobalInstance;
	class ISingleton abstract
	{
		friend class GlobalInstance;
	public:
		template <class T, class ...Args>
		static void Register(Args... args)
		{
			u64 hashCode = typeid(T).hash_code();

			if (true == GlobalInstance::Instance()->IsValid(hashCode))
			{
				auto instance = CreateSharedPtr<T>(args...);

				GlobalInstance::Instance()->RegisterInstance(hashCode, instance);

				instance->Init();
			}
		}
	};

	class GlobalInstance final
	{
		friend class ISingleton;
	private:
		Dictionary<u64, SharedPtr<ISingleton>> mInstances;
		static UniquePtr<GlobalInstance> mInstance;
	public:
		static void Create()
		{
			if (nullptr == mInstance)
			{
				mInstance = CreateUniquePtr<GlobalInstance>();
			}
		}
	public:
		static void Release()
		{
			mInstance->ReleaseInstance();
		}
	private:
		void RegisterInstance(u64 hashCode, SharedPtr<ISingleton> instance)
		{
			mInstances.emplace(hashCode, instance);
		}
		void ReleaseInstance()
		{
			for (auto& instance : mInstances)
			{
				instance.second.reset();
			}

			mInstance.reset();
		}
	public:
		template <class T>
		static SharedPtr<T> Instance()
		{
			return std::static_pointer_cast<T>(mInstance->Find(typeid(T).hash_code()));
		}
	private:
		static GlobalInstance* Instance()
		{
			return mInstance.get();
		}
		SharedPtr<ISingleton> Find(u64 hashCode)
		{
			return mInstances.find(hashCode)->second;
		}
		bool IsValid(u64 hashCode)
		{
			if (mInstances.find(hashCode) == mInstances.end() && nullptr != mInstance)
			{
				return true;
			}

			return false;
		}
	};
}