#include "pch.h"
#include "GlobalInstance.h"

namespace CLIENT
{
	UniquePtr<GlobalInstance> GlobalInstance::mInstance = nullptr;
}