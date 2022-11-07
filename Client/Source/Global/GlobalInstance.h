#pragma once

#include "Common/Define.h"

namespace CLIENT
{
	class ISingleton abstract
	{
	public:
		virtual ~ISingleton() = default;
	protected:
		virtual void Init() = 0;
	};

	class GlobalInstance final
	{
	private:
		static UniquePtr<GlobalInstance> mInstance;
	private:
		stack<u64> mRegistOrder;
		Dictionary<u64, SharedPtr<ISingleton>> mInstances;
	public:
		static void Init()
		{
			if (nullptr == mInstance)
			{
				mInstance = CreateUniquePtr<GlobalInstance>();
			}
		}
		static void Release()
		{
			mInstance->ReleaseInstance();
		}
		template <class T, class ...Args>
		static void Register(Args... args)
		{
			u64 hashCode = typeid(T).hash_code();

			if (nullptr == GlobalInstance::Instance()->FindInstance(hashCode))
			{
				auto instance = CreateSharedPtr<T>(args...);

				GlobalInstance::Instance()->RegisterInstance(hashCode, instance);

				instance->Init();
			}
		}
		template <class T>
		static SharedPtr<T> Instance()
		{
			return std::static_pointer_cast<T>(mInstance->FindInstance(typeid(T).hash_code()));
		}
		template <class T>
		static bool IsValid()
		{
			return mInstance->FindInstance(typeid(T).hash_code()) != nullptr;
		}
	private:
		static GlobalInstance* Instance()
		{
			return mInstance.get();
		}
		SharedPtr<ISingleton> FindInstance(u64 hashCode)
		{
			auto instance = mInstances.find(hashCode);

			if (instance == mInstances.end())
			{
				return nullptr;
			}

			return instance->second;
		}
		void RegisterInstance(u64 hashCode, SharedPtr<ISingleton> instance)
		{
			mInstances.emplace(hashCode, instance);
			mRegistOrder.push(hashCode);
		}
		void ReleaseInstance()
		{
			while (mRegistOrder.empty() == false)
			{
				auto order = mRegistOrder.top(); mRegistOrder.pop();

				auto instance = FindInstance(order);

				instance.reset();
			}

			mInstances.clear();
			mInstance.reset();
		}
	};
}