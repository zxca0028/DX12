#pragma once

#include "Core.h"

namespace CLIENT
{
	class SystemUI final : public ISingleton
	{
		friend class GlobalInstance;
		friend class Application;
	public:
		virtual ~SystemUI();
	private:
		virtual void Init() override;
	};
}