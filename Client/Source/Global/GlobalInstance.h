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
				auto instance = new T(args...);
				
				GlobalInstance::Instance()->RegisterInstance(hashCode, instance);
			}
		}
	protected:
		virtual void Release() = 0;
	};

	class GlobalInstance final
	{
		friend class ISingleton;
	private:
		Dictionary<u64, ISingleton*> mInstances;
		static GlobalInstance* mInstance;
	public:
		static void Create()
		{
			if (nullptr == mInstance)
			{
				mInstance = new GlobalInstance();
			}
		}
		static void Destroy()
		{
			if (nullptr != mInstance)
			{
				mInstance->ReleaseInstance();

				delete mInstance;
				mInstance = nullptr;
			}
		}
	private:
		void RegisterInstance(u64 hashCode, ISingleton* instance)
		{
			mInstances.emplace(hashCode, instance);
		}
		void ReleaseInstance()
		{
			for (auto& instance : mInstances)
			{
				instance.second->Release();

				delete instance.second;
				instance.second = nullptr;
			}

			mInstances.clear();
		}
	public:
		template <class T>
		static T* Instance()
		{
			return static_cast<T*>(mInstance->Find(typeid(T).hash_code()));
		}
	private:
		static GlobalInstance* Instance()
		{
			return mInstance;
		}
		ISingleton* Find(u64 hashCode)
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