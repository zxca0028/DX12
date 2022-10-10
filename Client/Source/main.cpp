#include "pch.h"
#include "Application.h"
#include "Core.h"

using namespace CLIENT;

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UniquePtr<Application> App = CreateUniquePtr<Application>();

	if (App->Create())
	{
		App->Run();
	}

	App->Destroy();

	return 0;
}