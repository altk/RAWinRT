#include "pch.h"
#include "memoryapi.h"
#include "RAWinRT.WRL.h"

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Local\\MyFileMappingObject");
TCHAR szMsg[] = TEXT("Message from first process.");

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;
using namespace ABI::RAWinRT::WRL;
using namespace ABI::Windows::ApplicationModel::Background;

class TestTaskImpl : public RuntimeClass < RuntimeClassFlags<WinRt>, IBackgroundTask >
{
	InspectableClass(RuntimeClass_RAWinRT_WRL_TestTask, BaseTrust);
public:
	STDMETHODIMP Run(IBackgroundTaskInstance *taskInstance) override
	{
		auto hMapFile = CreateFileMappingFromApp(
			INVALID_HANDLE_VALUE,
			nullptr,
			PAGE_READWRITE,
			BUF_SIZE,
			szName);

		auto pBuf = static_cast<LPTSTR>(MapViewOfFileFromApp(
			hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			BUF_SIZE
			));

		OutputDebugStringW(pBuf);

		UnmapViewOfFile(pBuf);

		CloseHandle(hMapFile);

		ComPtr<IBackgroundTaskDeferral> deferral;
		taskInstance->GetDeferral(
			deferral.GetAddressOf()
			);

		unsigned int progress;
		for (progress = 0; progress <= 100; progress += 10)
		{
			taskInstance->get_Progress(&progress);
		}

		deferral->Complete();
		return S_OK;
	}
};

ActivatableClass(TestTaskImpl);