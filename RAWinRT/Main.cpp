#include "pch.h"
#include "dx.h"
#include "SampleWindow.h"
#include <Windows.ApplicationModel.Core.h>
#pragma comment(lib, "RuntimeObject.lib")

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	using namespace Microsoft::WRL;
	using namespace ABI::Windows::ApplicationModel::Core;
	
	RoInitialize(RO_INIT_MULTITHREADED);
	
	auto app = ::dxdoth::GetActivationFactory<ICoreApplication>(
		RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);

	SampleWindow window;
	::dxdoth::HR(app->Run(&window));
}