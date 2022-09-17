#include "pch.h"
#include "Application.h"
#include "Core.h"

using namespace CLIENT;

int main()
{
	UniquePtr<Application> App = CreateUniquePtr<Application>();

	if (App->Create())
	{
		App->Run();
	}

	App->Destroy();

	return 0;
}