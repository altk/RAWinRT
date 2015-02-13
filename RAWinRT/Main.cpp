#include "pch.h"
#include "dx.h"
#include "memoryapi.h"
#include "SampleWindow.h"
#include <Windows.ApplicationModel.Core.h>
#pragma comment(lib, "RuntimeObject.lib")

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[] = TEXT("Message from first process.");

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	using namespace Microsoft::WRL;
	using namespace ABI::Windows::ApplicationModel::Core;
	
	RoInitialize(RO_INIT_MULTITHREADED);
	
	auto app = ::dxdoth::GetActivationFactory<ICoreApplication>(
		RuntimeClass_Windows_ApplicationModel_Core_CoreApplication);

	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMappingFromApp(
		INVALID_HANDLE_VALUE,    // use paging file
		nullptr,                    // default security
		PAGE_READWRITE,          // read/write access
		BUF_SIZE,                       // maximum object size (high-order DWORD)
		szName);          

	if (hMapFile == nullptr)
	{
		return 1;
	}

	pBuf = static_cast<LPTSTR>(MapViewOfFileFromApp(
		hMapFile,  
		FILE_MAP_ALL_ACCESS, 
		0,
		BUF_SIZE));

	if (pBuf==nullptr)
	{
		return 1;
	}

	auto size = _countof(szMsg)*sizeof(TCHAR);

	CopyMemory((PVOID)pBuf, szMsg, size);

	SampleWindow window;
	::dxdoth::HR(app->Run(&window));

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);
}